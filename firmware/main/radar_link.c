/*
 * Radar side of the flasher test: boot pins, reset, two UARTs, XMODEM sender,
 * TinyFrame version query. Protocol facts come from the radar's documentation, written here in
 * our own words; no third-party code is included.
 */
#include "radar_link.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_timer.h"
#include "logbuf.h"
#include "spi_rx.h"
#include "radar_cfg.h"
#include "usb_fwd.h"
#include "app_json.h"
#include "radar_link_priv.h"
#include "sdkconfig.h"

#define U_DATA UART_NUM_1   /* radar UART0: application TinyFrame traffic */
#define U_BOOT UART_NUM_2   /* radar UART1: ROM bootloader */
#define BAUD 115200

#define SOH 0x01
#define EOT 0x04
#define ACK 0x06
#define NAK 0x15

#define RAM_IMAGE_MAX 62976 /* ROM receive buffer: 492 packets x 128 B */

static SemaphoreHandle_t s_mu_data, s_mu_boot;
static QueueHandle_t s_jobs;
static volatile bool s_busy;

typedef struct { job_op_t op; char a[80]; char b[80]; } job_t;

int64_t now_ms(void) { return esp_timer_get_time() / 1000; }

/* ---------------- pins ---------------- */

static void od_pin_init(int gpio)
{
    if (gpio < 0) return;
    gpio_config_t c = {
        .pin_bit_mask = 1ULL << gpio,
        .mode = GPIO_MODE_INPUT_OUTPUT_OD,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
    };
    gpio_config(&c);
    gpio_set_level(gpio, 1); /* released: the radar's own pull-up decides */
}

static bool pins_wired(void) { return CONFIG_RB_P12_GPIO >= 0 && CONFIG_RB_P19_GPIO >= 0; }

static const char *mode_name(boot_mode_t m)
{
    return m == BOOT_FLASH ? "FLASH (P12 high, P19 low)" :
           m == BOOT_XMODEM ? "UART/XMODEM (P12 low, P19 low)" : "DEBUG (P12 high, P19 high)";
}

void set_boot(boot_mode_t m)
{
    if (!pins_wired()) {
        rlog("ACTION: set boot pins by hand to %s", mode_name(m));
        return;
    }
    /* open-drain: 0 = pull low, 1 = release (chip pull-up -> high) */
    gpio_set_level(CONFIG_RB_P12_GPIO, m == BOOT_XMODEM ? 0 : 1);
    gpio_set_level(CONFIG_RB_P19_GPIO, m == BOOT_DEBUG ? 1 : 0);
    rlog("boot pins -> %s", mode_name(m));
}

void release_boot_pins(void)
{
    if (!pins_wired()) return;
    /* Leave P19 low so a later reset still boots from flash; free P12. */
    gpio_set_level(CONFIG_RB_P12_GPIO, 1);
    gpio_set_level(CONFIG_RB_P19_GPIO, 0);
}

void pulse_reset(void)
{
    if (CONFIG_RB_RST_GPIO < 0) {
        rlog("ACTION: press the radar reset (or power-cycle it) within 10 s");
        return;
    }
    gpio_set_level(CONFIG_RB_RST_GPIO, 0);
    vTaskDelay(pdMS_TO_TICKS(20));
    gpio_set_level(CONFIG_RB_RST_GPIO, 1);
    rlog("reset pulsed");
}

void radar_pin_probe(void)
{
    static const struct { int gpio; const char *name; } p[] = {
        { CONFIG_RB_U1_TX_GPIO, "boot TX ->radar" }, { CONFIG_RB_U1_RX_GPIO, "boot RX <-radar" },
        { CONFIG_RB_U0_TX_GPIO, "app TX ->radar" }, { CONFIG_RB_U0_RX_GPIO, "app RX <-radar" },
        { CONFIG_RB_RST_GPIO, "RST" }, { CONFIG_RB_P12_GPIO, "P12" }, { CONFIG_RB_P19_GPIO, "P19" },
        { CONFIG_RB_SPI_SCLK_GPIO, "SPI CLK" }, { CONFIG_RB_SPI_DATA_GPIO, "SPI MOSI" },
    };
    const int n = sizeof p / sizeof p[0];
    int hi[sizeof p / sizeof p[0]] = {0}, lo[sizeof p / sizeof p[0]] = {0};
    for (int i = 0; i < n; i++) gpio_input_enable(p[i].gpio);
    static const uint8_t pat[64] = { [0 ... 63] = 0x55 };
    xSemaphoreTake(s_mu_boot, portMAX_DELAY);
    for (int round = 0; round < 40; round++) {
        uart_write_bytes(U_BOOT, pat, sizeof pat);
        for (int k = 0; k < 50; k++)
            for (int i = 0; i < n; i++) { if (gpio_get_level(p[i].gpio)) hi[i]++; else lo[i]++; }
        vTaskDelay(1);
    }
    uart_wait_tx_done(U_BOOT, pdMS_TO_TICKS(500));
    uart_flush_input(U_BOOT);
    xSemaphoreGive(s_mu_boot);
    for (int i = 0; i < n; i++)
        rlog("pins: GPIO%-2d %-16s high %4d low %4d -> %s", p[i].gpio, p[i].name, hi[i], lo[i],
             !lo[i] ? "always high" : !hi[i] ? "always LOW" : "toggling");
}

const char *radar_pin_summary(void)
{
    static char s[200];
    snprintf(s, sizeof s,
             "RST=%d P12=%d P19=%d | radar UART0: ESP TX%d RX%d | radar UART1: ESP TX%d RX%d",
             CONFIG_RB_RST_GPIO, CONFIG_RB_P12_GPIO, CONFIG_RB_P19_GPIO,
             CONFIG_RB_U0_TX_GPIO, CONFIG_RB_U0_RX_GPIO, CONFIG_RB_U1_TX_GPIO, CONFIG_RB_U1_RX_GPIO);
    return s;
}

/* ---------------- text capture ---------------- */

/* Accumulates printable bytes into lines and logs them with a prefix. */
typedef struct { char buf[120]; int n; const char *tag; } line_acc_t;

static void acc_flush(line_acc_t *a)
{
    if (a->n) { a->buf[a->n] = 0; rlog("%s| %s", a->tag, a->buf); a->n = 0; }
}

static void acc_byte(line_acc_t *a, uint8_t c)
{
    if (c == '\n' || c == '\r') { acc_flush(a); return; }
    if (a->n >= (int)sizeof a->buf - 6) acc_flush(a);
    if (c >= 0x20 && c < 0x7f) a->buf[a->n++] = c;
    else a->n += snprintf(a->buf + a->n, sizeof a->buf - a->n, "<%02X>", c);
}

/* Wait until the receiver on `u` sends a lone 'C' (XMODEM-CRC request).
   Text before it (boot banners) is logged. */
bool wait_for_C(uart_port_t u, const char *tag, int timeout_ms)
{
    line_acc_t a = { .tag = tag };
    int64_t end = now_ms() + timeout_ms;
    uint8_t c;
    while (now_ms() < end) {
        if (uart_read_bytes(u, &c, 1, pdMS_TO_TICKS(50)) != 1) continue;
        if (c == 'C') {
            uint8_t nx;
            /* a real request is followed by silence; banner text is not */
            if (uart_read_bytes(u, &nx, 1, pdMS_TO_TICKS(30)) != 1) {
                acc_flush(&a);
                rlog("%s| <got 'C' = XMODEM-CRC request>", tag);
                return true;
            }
            acc_byte(&a, c);
            acc_byte(&a, nx);
            continue;
        }
        acc_byte(&a, c);
    }
    acc_flush(&a);
    return false;
}

/* Log text from `u` until `needle` is seen or timeout. */
bool wait_for_text(uart_port_t u, const char *tag, const char *needle, int timeout_ms)
{
    line_acc_t a = { .tag = tag };
    char hist[64] = {0};
    int64_t end = now_ms() + timeout_ms;
    uint8_t c;
    while (now_ms() < end) {
        if (uart_read_bytes(u, &c, 1, pdMS_TO_TICKS(50)) != 1) continue;
        acc_byte(&a, c);
        memmove(hist, hist + 1, sizeof hist - 2);
        hist[sizeof hist - 2] = (char)c;
        if (strstr(hist, needle)) { acc_flush(&a); return true; }
    }
    acc_flush(&a);
    return false;
}

/* ---------------- XMODEM-CRC sender (128-byte packets) ---------------- */

uint16_t crc16(const uint8_t *p, int n)
{
    uint16_t crc = 0;
    while (n--) {
        crc ^= (uint16_t)(*p++) << 8;
        for (int i = 0; i < 8; i++) crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
    }
    return crc;
}

/* allow_retry=false for the ROM: its receiver cannot recover from a NAK. */
bool xmodem_send(uart_port_t u, const uint8_t *data, size_t len, bool allow_retry, const char *tag)
{
    uint8_t pkt[133];
    uint8_t no = 1;
    size_t npk = (len + 127) / 128;
    int64_t t0 = now_ms();
    for (size_t i = 0; i < npk; i++, no++) {
        size_t off = i * 128, n = len - off > 128 ? 128 : len - off;
        pkt[0] = SOH; pkt[1] = no; pkt[2] = (uint8_t)~no;
        memset(pkt + 3, 0xFF, 128);
        memcpy(pkt + 3, data + off, n);
        uint16_t c = crc16(pkt + 3, 128);
        pkt[131] = c >> 8; pkt[132] = c & 0xFF;
        for (int tries = 0;; tries++) {
            uart_flush_input(u);
            uart_write_bytes(u, pkt, sizeof pkt);
            uint8_t r = 0;
            int got;
            int64_t until = now_ms() + 3000;
            do { /* ignore a late 'C' the receiver may still emit */
                got = uart_read_bytes(u, &r, 1, pdMS_TO_TICKS(100));
            } while ((got != 1 || r == 'C') && now_ms() < until);
            if (r == 'C') got = 0;
            if (got == 1 && r == ACK) break;
            if (got != 1) rlog("%s: packet %u: no reply", tag, (unsigned)(i + 1));
            else rlog("%s: packet %u: reply 0x%02X (expected ACK)", tag, (unsigned)(i + 1), r);
            if (!allow_retry || tries >= 3) return false;
        }
        if ((i + 1) % 64 == 0 || i + 1 == npk)
            rlog("%s: %u/%u packets", tag, (unsigned)(i + 1), (unsigned)npk);
    }
    uint8_t eot = EOT, r = 0;
    uart_write_bytes(u, &eot, 1);
    if (uart_read_bytes(u, &r, 1, pdMS_TO_TICKS(3000)) != 1 || r != ACK) {
        rlog("%s: EOT not acknowledged (0x%02X)", tag, r);
        return false;
    }
    rlog("%s: %u bytes sent in %lld ms", tag, (unsigned)len, now_ms() - t0);
    return true;
}

/* ---------------- files ---------------- */

uint8_t *load_file(const char *name, size_t *len)
{
    char path[128];
    snprintf(path, sizeof path, "/bins/%s", name);
    FILE *f = fopen(path, "rb");
    if (!f) { rlog("cannot open %s", path); return NULL; }
    struct stat st;
    stat(path, &st);
    uint8_t *b = malloc(st.st_size ? st.st_size : 1);
    *len = fread(b, 1, st.st_size, f);
    fclose(f);
    return b;
}

/* Images carry a 16-byte header: len16, len16, 0x5A, 24-bit code offset, clk div. */
bool has_header(const uint8_t *b, size_t len, uint32_t *code_len, uint32_t *code_off)
{
    if (len < 16 || b[4] != 0x5A) return false;
    uint32_t l1 = b[0] | b[1] << 8, l2 = b[2] | b[3] << 8;
    uint32_t off = b[5] | b[6] << 8 | b[7] << 16;
    if (l1 != l2 || off + l1 > len) return false;
    *code_len = l1; *code_off = off;
    return true;
}

/* ---------------- TinyFrame (radar UART0) ---------------- */

static void tf_send(uint16_t type, const uint8_t *data, uint16_t len)
{
    static uint16_t id = 0x8001;
    uint8_t h[8] = { 0x01, id >> 8, id & 0xFF, len >> 8, len & 0xFF, type >> 8, type & 0xFF, 0 };
    uint8_t x = 0;
    for (int i = 0; i < 7; i++) x ^= h[i];
    h[7] = ~x;
    uart_write_bytes(U_DATA, h, 8);
    if (len) {
        x = 0;
        for (int i = 0; i < len; i++) x ^= data[i];
        x = ~x;
        uart_write_bytes(U_DATA, data, len);
        uart_write_bytes(U_DATA, &x, 1);
    }
    id = id == 0xFFFF ? 0x8001 : id + 1;
}

typedef struct {
    int st, need, got;
    uint8_t h[8];
    uint8_t pl[2048];     /* point clouds can be long */
    uint16_t len, type;
} tf_rx_t;

void radar_tf_send(uint16_t type, const uint8_t *data, uint16_t len) { tf_send(type, data, len); app_json_on_cmd(type, data, len); }

static volatile int s_ver_seen;
static uint32_t s_type_count[16];
static uint16_t s_type_id[16];

static void tf_frame(uint16_t type, const uint8_t *p, uint16_t len, bool trunc)
{
    if (!trunc) usb_fwd_tf(type, p, len);   /* app results to the PC GUI (appout on) */
    if (!trunc) app_json_on_tf(type, p, len);   /* schema v0 JSON (jsonout on / web page) */
    if (type == 0xFFFF || type == 0x3000) {
        char hex[100] = {0};
        for (int i = 0; i < len && i < 24; i++) sprintf(hex + i * 3, "%02X ", p[i]);
        rlog("U0| TinyFrame type 0x%04X len %u: %s%s", type, len, hex, trunc ? "..." : "");
        if (type == 0xFFFF) s_ver_seen++;
        return;
    }
    for (int i = 0; i < 16; i++) {
        if (s_type_count[i] && s_type_id[i] != type) continue;
        if (!s_type_count[i]) s_type_id[i] = type;
        s_type_count[i]++;
        return;
    }
}

static void tf_byte(tf_rx_t *r, uint8_t c, line_acc_t *txt)
{
    switch (r->st) {
    case 0:
        if (c == 0x01) { r->h[0] = c; r->got = 1; r->st = 1; }
        else acc_byte(txt, c); /* plain text, e.g. debug prints */
        break;
    case 1:
        r->h[r->got++] = c;
        if (r->got == 8) {
            uint8_t x = 0;
            for (int i = 0; i < 7; i++) x ^= r->h[i];
            if ((uint8_t)~x != r->h[7]) { r->st = 0; break; }
            r->len = r->h[3] << 8 | r->h[4];
            r->type = r->h[5] << 8 | r->h[6];
            r->got = 0;
            if (r->len == 0) { tf_frame(r->type, NULL, 0, false); r->st = 0; }
            else r->st = 2;
        }
        break;
    case 2:
        if (r->got < (int)sizeof r->pl) r->pl[r->got] = c;
        r->got++;
        if (r->got == r->len + 1) { /* payload + checksum */
            int n = r->len < sizeof r->pl ? r->len : sizeof r->pl;
            tf_frame(r->type, r->pl, n, r->len > sizeof r->pl);
            r->st = 0;
        }
        break;
    }
}

static void data_monitor_task(void *arg)
{
    tf_rx_t r = {0};
    line_acc_t txt = { .tag = "U0 text" };
    uint8_t buf[256];
    int64_t last = now_ms();
    for (;;) {
        xSemaphoreTake(s_mu_data, portMAX_DELAY);
        int n = uart_read_bytes(U_DATA, buf, sizeof buf, pdMS_TO_TICKS(20));
        xSemaphoreGive(s_mu_data);
        if (n > 0) usb_fwd_raw(buf, n);   /* `u0raw on`: the radar app port byte for byte */
        for (int i = 0; i < n; i++) tf_byte(&r, buf[i], &txt);
        if (n <= 0) acc_flush(&txt);
        if (now_ms() - last > 5000) {
            char s[160] = {0};
            int k = 0;
            for (int i = 0; i < 16 && s_type_count[i]; i++)
                k += snprintf(s + k, sizeof s - k, "0x%04X x%lu ", s_type_id[i], (unsigned long)s_type_count[i]);
            if (k) rlog("U0| frames in last 5 s: %s", s);
            memset(s_type_count, 0, sizeof s_type_count);
            last = now_ms();
        }
        vTaskDelay(1);
    }
}

static void boot_monitor_task(void *arg)
{
    line_acc_t txt = { .tag = "U1" };
    uint8_t buf[128];
    for (;;) {
        xSemaphoreTake(s_mu_boot, portMAX_DELAY);
        int n = uart_read_bytes(U_BOOT, buf, sizeof buf, pdMS_TO_TICKS(20));
        xSemaphoreGive(s_mu_boot);
        for (int i = 0; i < n; i++) acc_byte(&txt, buf[i]);
        if (n <= 0) acc_flush(&txt);
        vTaskDelay(1);
    }
}

/* ---------------- jobs ---------------- */

bool query_version(void)
{
    int before = s_ver_seen;
    rlog("sending version query (TinyFrame type 0xFFFF) on radar UART0");
    tf_send(0xFFFF, NULL, 0);
    for (int i = 0; i < 20; i++) {
        vTaskDelay(pdMS_TO_TICKS(100));
        if (s_ver_seen != before) { rlog("RESULT: app answered the version query"); return true; }
    }
    rlog("RESULT: no version reply within 2 s");
    return false;
}

/* Put the chip into ROM UART boot and wait for its 'C'. Caller holds s_mu_boot. */
bool enter_rom_xmodem(void)
{
    set_boot(BOOT_XMODEM);
    uart_flush_input(U_BOOT);
    pulse_reset();
    rlog("waiting up to 15 s for the ROM banner and 'C' on radar UART1 (press reset by hand if RST is not wired)");
    if (!wait_for_C(U_BOOT, "U1", 15000)) {
        rlog("FAIL: no XMODEM request from the ROM on radar UART1. Check P12/P19 low, UART1 wiring, GND.");
        return false;
    }
    return true;
}

static void job_rom_probe(void)
{
    xSemaphoreTake(s_mu_boot, portMAX_DELAY);
    bool ok = enter_rom_xmodem();
    xSemaphoreGive(s_mu_boot);
    rlog(ok ? "RESULT: ROM UART boot works (banner + 'C' seen). Radar now waits for an image; reset to leave."
            : "RESULT: ROM probe failed");
}

/* ROM-load `file` into radar RAM and start it. Boot pins are released afterwards. */
static bool ram_load(const char *file)
{
    size_t len;
    uint8_t *b = load_file(file, &len);
    if (!b) return false;
    uint32_t cl, co;
    const uint8_t *img = b;
    size_t ilen = len;
    if (has_header(b, len, &cl, &co)) {
        img = b + co; ilen = cl;
        rlog("%s: header found, sending %u code bytes from offset %u", file, (unsigned)cl, (unsigned)co);
    } else {
        rlog("%s: no header, sending file as a raw RAM image", file);
    }
    if (ilen > RAM_IMAGE_MAX) { rlog("FAIL: %u bytes > ROM limit %u", (unsigned)ilen, RAM_IMAGE_MAX); free(b); return false; }

    xSemaphoreTake(s_mu_boot, portMAX_DELAY);
    bool ok = enter_rom_xmodem() && xmodem_send(U_BOOT, img, ilen, false, "ROM xmodem");
    if (ok) wait_for_text(U_BOOT, "U1", "successful", 1500);
    xSemaphoreGive(s_mu_boot);
    free(b);
    release_boot_pins();
    if (!ok) rlog("RESULT: RAM load failed (reset the radar before retrying)");
    return ok;
}

static void job_ram_run(const char *file)
{
    if (!ram_load(file)) return;
    rlog("image is running from RAM (flash untouched). Waiting 1.5 s for the app to start...");
    vTaskDelay(pdMS_TO_TICKS(1500));
    query_version();
}

static volatile bool s_streaming;

void radar_cfg_push(void)
{
    if (!s_streaming) { rlog("cfg: saved; it is sent to the radar when a stream starts"); return; }
    uint8_t msg[64];
    size_t n = radar_cfg_encode(radar_cfg_get(), msg, sizeof msg);
    uart_write_bytes(U_BOOT, msg, n);
    rlog("cfg: sent to the radar (%u bytes), waiting for its '[KYM] cfg ok' line", (unsigned)n);
}

/* Radar SPI0 -> ESP32 SPI slave: RAM-run the radar app, push the stream config, then receive.
   P12 is the radar's chip select after boot, so its ESP32 pin becomes an input meanwhile. */
static void job_spi_test(const char *file, const char *secs)
{
    int s = secs[0] ? atoi(secs) : 30;   /* 0 = run until "stop" */
    if (s < 0) s = 30;
    bool loaded = false;
    for (int attempt = 1; attempt <= 3 && !loaded; attempt++) {
        if (attempt > 1) rlog("spi test: retrying the RAM load (attempt %d of 3)", attempt);
        loaded = ram_load(file);
    }
    if (!loaded) return;
    gpio_set_direction(CONFIG_RB_P12_GPIO, GPIO_MODE_INPUT);
    s_streaming = true;
    vTaskDelay(pdMS_TO_TICKS(300));      /* let the radar app finish its init */
    radar_cfg_push();                    /* plain test programs simply ignore it */
    spi_rx_run(CONFIG_RB_SPI_SCLK_GPIO, CONFIG_RB_SPI_DATA_GPIO, CONFIG_RB_P12_GPIO, s);
    s_streaming = false;
    od_pin_init(CONFIG_RB_P12_GPIO);
    set_boot(BOOT_FLASH);
    pulse_reset();
    rlog("spi test over; rebooting the radar from flash");
    vTaskDelay(pdMS_TO_TICKS(1500));
    query_version();
}

/* Look for the loader's 'C' on either radar UART. */
uart_port_t wait_loader_C(int timeout_ms)
{
    int64_t end = now_ms() + timeout_ms;
    while (now_ms() < end) {
        if (wait_for_C(U_BOOT, "U1 loader", 300)) return U_BOOT;
        if (wait_for_C(U_DATA, "U0 loader", 300)) return U_DATA;
    }
    return UART_NUM_MAX;
}

static void job_ota_probe(void)
{
    rlog("sending in-app OTA request (TinyFrame type 0x3000). Only meaningful if the module has the two-stage OTA boot.");
    tf_send(0x3000, NULL, 0);
    xSemaphoreTake(s_mu_boot, portMAX_DELAY);
    xSemaphoreTake(s_mu_data, portMAX_DELAY);
    uart_port_t u = wait_loader_C(6000);
    xSemaphoreGive(s_mu_data);
    xSemaphoreGive(s_mu_boot);
    if (u == UART_NUM_MAX) rlog("RESULT: no XMODEM request after OTA command -> module has no OTA boot (normal). Reset the radar.");
    else rlog("RESULT: module asked for an image on %s after 0x3000 -> in-app OTA boot present. Reset the radar to cancel.",
              u == U_BOOT ? "UART1" : "UART0");
}

static void worker_task(void *arg)
{
    job_t j;
    for (;;) {
        xQueueReceive(s_jobs, &j, portMAX_DELAY);
        s_busy = true;
        rlog("==== job start: %d %s %s", j.op, j.a, j.b);
        switch (j.op) {
        case JOB_VERSION: query_version(); break;
        case JOB_RESET_FLASH: set_boot(BOOT_FLASH); pulse_reset(); vTaskDelay(pdMS_TO_TICKS(1500)); query_version(); break;
        case JOB_ROM_PROBE: job_rom_probe(); break;
        case JOB_RAM_RUN: job_ram_run(j.a); break;
        case JOB_FLASH: case JOB_BACKUP: case JOB_BATCH:
            rlog("RESULT: writing or backing up the radar flash needs the dev-kit firmware; this open build only runs apps from RAM");
            break;
        case JOB_OTA_PROBE: job_ota_probe(); break;
        case JOB_SPI_TEST: job_spi_test(j.a, j.b); break;
        default: break;
        }
        rlog("==== job end");
        s_busy = false;
    }
}

bool radar_busy(void) { return s_busy; }

void radar_lock_uarts(void) { xSemaphoreTake(s_mu_boot, portMAX_DELAY); xSemaphoreTake(s_mu_data, portMAX_DELAY); }
void radar_unlock_uarts(void) { xSemaphoreGive(s_mu_data); xSemaphoreGive(s_mu_boot); }

bool radar_submit(job_op_t op, const char *a, const char *b)
{
    if (s_busy || uxQueueMessagesWaiting(s_jobs)) return false;
    job_t j = { .op = op };
    if (a) strlcpy(j.a, a, sizeof j.a);
    if (b) strlcpy(j.b, b, sizeof j.b);
    return xQueueSend(s_jobs, &j, 0) == pdTRUE;
}

static void uart_setup(uart_port_t u, int tx, int rx)
{
    uart_config_t c = {
        .baud_rate = BAUD, .data_bits = UART_DATA_8_BITS, .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1, .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, .source_clk = UART_SCLK_DEFAULT,
    };
    ESP_ERROR_CHECK(uart_driver_install(u, 4096, 1024, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(u, &c));
    ESP_ERROR_CHECK(uart_set_pin(u, tx, rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    gpio_pullup_en(rx); /* keep RX idle-high when the lead is unplugged */
}

void radar_init(void)
{
    od_pin_init(CONFIG_RB_RST_GPIO);
    od_pin_init(CONFIG_RB_P12_GPIO);
    od_pin_init(CONFIG_RB_P19_GPIO);
    if (pins_wired()) gpio_set_level(CONFIG_RB_P19_GPIO, 0); /* default: flash boot */
    uart_setup(U_DATA, CONFIG_RB_U0_TX_GPIO, CONFIG_RB_U0_RX_GPIO);
    uart_setup(U_BOOT, CONFIG_RB_U1_TX_GPIO, CONFIG_RB_U1_RX_GPIO);
    s_mu_data = xSemaphoreCreateMutex();
    s_mu_boot = xSemaphoreCreateMutex();
    s_jobs = xQueueCreate(1, sizeof(job_t));
    xTaskCreate(worker_task, "rb_worker", 8192, NULL, 5, NULL);
    xTaskCreate(data_monitor_task, "rb_u0", 6144, NULL, 4, NULL);   /* + schema v0 JSON building (app_json.c) */
    xTaskCreate(boot_monitor_task, "rb_u1", 4096, NULL, 4, NULL);
    rlog("pins: %s", radar_pin_summary());
}
