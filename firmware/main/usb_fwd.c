#include "usb_fwd.h"
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "driver/usb_serial_jtag.h"
#include "esp_heap_caps.h"
#include "esp_rom_crc.h"
#include "logbuf.h"

#define FWD_MAX (160 * 1024)

static SemaphoreHandle_t s_out_mu, s_ready;
static volatile bool s_on, s_busy;
static uint8_t *s_buf;
static uint32_t s_len, s_frame;
static adc_hdr_t s_hdr;
static bool s_assembling;
static uint32_t s_sent, s_skipped;

void usb_out_lock(void) { if (s_out_mu) xSemaphoreTake(s_out_mu, portMAX_DELAY); }
void usb_out_unlock(void) { if (s_out_mu) xSemaphoreGive(s_out_mu); }

static void put(const void *p, size_t n)
{
    const uint8_t *b = p;
    while (n) {
        int k = usb_serial_jtag_write_bytes(b, n > 4096 ? 4096 : n, pdMS_TO_TICKS(500));
        if (k <= 0) return;   /* host not reading: drop the rest of this packet */
        b += k; n -= k;
    }
}

static void sender_task(void *arg)
{
    for (;;) {
        xSemaphoreTake(s_ready, portMAX_DELAY);
        uint8_t h[24];
        memcpy(h, "KYF2", 4);
        memcpy(h + 4, &s_frame, 4);
        h[8] = s_hdr.chip; h[9] = s_hdr.format;
        uint16_t smp = (uint16_t)s_hdr.samples, chp = s_hdr.chirps, rsv = 0;
        memcpy(h + 10, &smp, 2); memcpy(h + 12, &chp, 2); memcpy(h + 14, &rsv, 2);
        memcpy(h + 16, &s_len, 4);
        uint32_t crc = esp_rom_crc32_le(0, s_buf, s_len);
        memcpy(h + 20, &crc, 4);   /* CRC first in the header copy, repeated after the payload */
        usb_out_lock();
        put(h, 20);
        put(s_buf, s_len);
        put(&crc, 4);
        usb_out_unlock();
        s_sent++;
        s_busy = false;
    }
}

void usb_fwd_init(void)
{
    s_out_mu = xSemaphoreCreateMutex();
    s_ready = xSemaphoreCreateBinary();
    xTaskCreate(sender_task, "usb_fwd", 4096, NULL, 3, NULL);
}

void usb_fwd_enable(bool on)
{
    if (on && !s_buf) s_buf = heap_caps_malloc(FWD_MAX, MALLOC_CAP_SPIRAM);
    s_on = on && s_buf;
    rlog("usbout: %s (frames sent %lu, skipped while busy %lu)", s_on ? "on" : "off",
         (unsigned long)s_sent, (unsigned long)s_skipped);
}

bool usb_fwd_enabled(void) { return s_on; }

static volatile bool s_tf_on;

void usb_fwd_tf_enable(bool on)
{
    s_tf_on = on;
    rlog("appout: %s", on ? "on (radar app TinyFrame messages forwarded to USB)" : "off");
}

void usb_fwd_tf(uint16_t type, const uint8_t *payload, uint16_t len)
{
    if (!s_tf_on) return;
    uint8_t h[8] = { 'K', 'Y', 'T', '1', (uint8_t)type, (uint8_t)(type >> 8), (uint8_t)len, (uint8_t)(len >> 8) };
    uint32_t crc = esp_rom_crc32_le(0, payload, len);
    usb_out_lock();
    put(h, sizeof h);
    if (len) put(payload, len);
    put(&crc, 4);
    usb_out_unlock();
}

static volatile bool s_raw_on;
void usb_fwd_raw_enable(bool on) { s_raw_on = on; rlog("u0raw: %s", on ? "on (radar UART0 bytes forwarded as KYR1)" : "off"); }
void usb_fwd_raw(const uint8_t *b, int n)
{
    if (!s_raw_on || n <= 0) return;
    uint8_t h[6] = { 'K', 'Y', 'R', '1', (uint8_t)n, (uint8_t)(n >> 8) };
    usb_out_lock();
    put(h, sizeof h);
    put(b, n);
    usb_out_unlock();
}

void usb_fwd_text(const char *s, int n)
{
    usb_out_lock();
    put(s, n);
    usb_out_unlock();
}

void usb_fwd_offer_chunk(const adc_hdr_t *h, const uint8_t *payload)
{
    if (!s_on) return;
    if (h->chunk == 0) {
        if (s_busy) { s_skipped++; s_assembling = false; return; }
        s_assembling = true; s_len = 0; s_frame = h->frame; s_hdr = *h;
    }
    if (!s_assembling || h->frame != s_frame || s_len != (uint32_t)h->chunk * 16384u || s_len + h->len > FWD_MAX) {
        s_assembling = false;
        return;
    }
    memcpy(s_buf + s_len, payload, h->len);
    s_len += h->len;
    if (h->chunk + 1 == h->nchunks) {
        s_assembling = false;
        s_busy = true;
        xSemaphoreGive(s_ready);
    }
}
