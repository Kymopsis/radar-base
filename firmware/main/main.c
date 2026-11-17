/*
 * Kymopsis Radar Base - flasher bench test (ESP32-S3 DevKit N16R8).
 * Loads radar application images into the radar chip over UART:
 *   - RAM run: ROM UART boot + XMODEM, flash untouched
 *   - Flash:   RAM-load the flash-writer loader, then XMODEM the app to it
 * Control: web app on the home network (http://kymradar.local), hotspot fallback
 * "KymRadarBase-xxxx" -> http://192.168.4.1, or the USB console.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_mac.h"
#include "esp_spiffs.h"
#include "nvs_flash.h"
#include "driver/usb_serial_jtag.h"
#include "logbuf.h"
#include "app_json.h"
#include "radar_link.h"
#include "radar_cfg.h"
#include "spi_rx.h"
#include "usb_fwd.h"
#include "wifi.h"
#include "mqtt_ha.h"
#include "board.h"
#include "sdkconfig.h"

void web_start(void);
void web_files_json(char *out);

/* `api files|cfg|wifi|scan`: the web app's JSON over the USB console, one line "@api <name> <json>",
   so the same app works over USB (Web Serial) without Wi-Fi. */
static void api_command(const char *name)
{
    char *j = malloc(8192);
    if (!j) return;
    if (!strcmp(name, "files")) web_files_json(j);
    else if (!strcmp(name, "cfg")) radar_cfg_json(j, 8192);
    else if (!strcmp(name, "wifi")) wifi_status_json(j, 8192);
    else if (!strcmp(name, "scan")) wifi_scan_json(j, 8192);
    else if (!strcmp(name, "mqtt")) mqtt_ha_status_json(j, 8192);
    else if (!strcmp(name, "batch")) snprintf(j, 8192, "{\"available\":false}");
    else snprintf(j, 8192, "{\"error\":\"unknown\"}");
    char *line = malloc(8300);
    if (line) {
        int n = snprintf(line, 8300, "@api %s %s\n", name, j);
        usb_fwd_text(line, n);
        free(line);
    }
    free(j);
}
void list_bins(void);
void get_file(const char *name);

static void storage_mount(void)
{
    esp_vfs_spiffs_conf_t c = { .base_path = "/bins", .partition_label = "storage", .max_files = 4, .format_if_mount_failed = true };
    ESP_ERROR_CHECK(esp_vfs_spiffs_register(&c));
}

/* USB console: ver | reset | rom | ram <file> | flash [loader] <app> | backup [loader] | ota | ls
   (loader defaults to the built-in one) */
static void console_task(void *arg)
{
    usb_serial_jtag_driver_config_t c = USB_SERIAL_JTAG_DRIVER_CONFIG_DEFAULT();
    c.tx_buffer_size = 16384;   /* room for binary frame forwarding (usbout) */
    usb_serial_jtag_driver_install(&c);
    char line[200];
    int n = 0;
    for (;;) {
        uint8_t ch;
        if (usb_serial_jtag_read_bytes(&ch, 1, portMAX_DELAY) != 1) continue;
        if (ch != '\r' && ch != '\n') { if (n < (int)sizeof line - 1) line[n++] = ch; continue; }
        line[n] = 0;
        n = 0;
        char cmd[16] = {0}, a[80] = {0}, b[80] = {0};
        if (sscanf(line, "%15s %79s %79s", cmd, a, b) < 1) continue;
        if (!strcmp(cmd, "stream")) strcpy(cmd, "spitest");
        job_op_t op = !strcmp(cmd, "ver") ? JOB_VERSION : !strcmp(cmd, "reset") ? JOB_RESET_FLASH :
                      !strcmp(cmd, "rom") ? JOB_ROM_PROBE : !strcmp(cmd, "ram") ? JOB_RAM_RUN :
                      !strcmp(cmd, "flash") ? JOB_FLASH : !strcmp(cmd, "ota") ? JOB_OTA_PROBE :
                      !strcmp(cmd, "backup") ? JOB_BACKUP : !strcmp(cmd, "spitest") ? JOB_SPI_TEST : JOB_NONE;
        if (!strcmp(cmd, "ls")) { list_bins(); continue; }
        if (!strcmp(cmd, "get")) { get_file(a); continue; }
        if (!strcmp(cmd, "pins")) { radar_pin_probe(); continue; }
        if (!strcmp(cmd, "wifi")) { wifi_command(a, b); continue; }
        if (!strcmp(cmd, "api")) { api_command(a); continue; }
        if (!strcmp(cmd, "board")) {   /* board [<profile id> | unknown] */
            if (a[0] && board_set(a)) rlog("board: unknown profile id (known: kymopsis-devkit-60g, kymopsis-diy-60g)");
            else if (!a[0]) { const board_profile_t *bp = board_get(); rlog("board: %s", bp ? bp->name : "Unknown board, angles not calibrated"); }
            continue;
        }
        if (!strcmp(cmd, "batch")) {   /* batch start <firmware> [nobackup] | batch stop */
            rlog("batch flashing is included with the Kymopsis development kit firmware");
            continue;
        }
        if (!strcmp(cmd, "mqtt")) {   /* mqtt <host> [port] [user] [pass] | on | off | dump */
            char c3[64] = {0}, c4[64] = {0};
            sscanf(line, "%*s %*s %*s %63s %63s", c3, c4);
            mqtt_ha_command(a, b, c3, c4);
            continue;
        }
        if (!strcmp(cmd, "cfg")) { radar_cfg_command(a, b); continue; }
        if (!strcmp(cmd, "stop")) { spi_rx_request_stop(); continue; }
        if (!strcmp(cmd, "usbout")) { usb_fwd_enable(strcmp(a, "off") != 0); continue; }
        if (!strcmp(cmd, "appout")) { usb_fwd_tf_enable(strcmp(a, "off") != 0); continue; }
        if (!strcmp(cmd, "u0raw")) { usb_fwd_raw_enable(strcmp(a, "off") != 0); continue; }
        if (!strcmp(cmd, "jsonout")) { app_json_usb_enable(strcmp(a, "off") != 0); continue; }
        if (!strcmp(cmd, "tf")) {   /* tf <type hex> [payload hex] -> radar app UART0 */
            uint8_t pl[64];
            size_t n = 0;
            for (const char *p = b; p[0] && p[1] && n < sizeof pl; p += 2) {
                char hx[3] = { p[0], p[1], 0 };
                pl[n++] = (uint8_t)strtoul(hx, NULL, 16);
            }
            uint16_t type = (uint16_t)strtoul(a, NULL, 16);
            radar_tf_send(type, pl, n);
            rlog("tf: sent type 0x%04X, %u bytes", type, (unsigned)n);
            continue;
        }
        if (!strcmp(cmd, "dump")) { spi_rx_request_dump(); rlog("dump: one frame of the next/current stream will be printed when it ends"); continue; }
        if (op == JOB_NONE) { rlog("commands: ver | reset | rom | ram <file> | flash [loader] <app> | backup [loader] | stream <file> [s] | cfg [key value] | pins | ota | ls"); continue; }
        /* the loader argument is optional: default is the built-in one */
        if (op == JOB_FLASH && !b[0]) { strcpy(b, a); strcpy(a, BUILTIN_LOADER); }
        if (op == JOB_BACKUP && !a[0]) strcpy(a, BUILTIN_LOADER);
        if (!radar_submit(op, a, b)) rlog("busy");
    }
}

#include <dirent.h>
#include <sys/stat.h>
void list_bins(void)
{
    DIR *d = opendir("/bins");
    struct dirent *e;
    while (d && (e = readdir(d))) {
        char p[300];
        struct stat st;
        snprintf(p, sizeof p, "/bins/%s", e->d_name);
        if (!stat(p, &st)) rlog("  %-50s %7ld", e->d_name, (long)st.st_size);
    }
    if (d) closedir(d);
}

/* get <file>: print a stored file to the USB console as hex so the PC can save it
   (e.g. flash backups). Format: FILEDUMP BEGIN <name> <len> / hex lines / FILEDUMP END <crc32 hex>. */
#include "esp_rom_crc.h"
void get_file(const char *name)
{
    char p[300];
    snprintf(p, sizeof p, "/bins/%s", name);
    FILE *f = fopen(p, "rb");
    if (!f) { rlog("get: no file %s", name); return; }
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    uint8_t buf[64];
    uint32_t crc = 0;
    usb_out_lock();
    printf("FILEDUMP BEGIN %s %ld\n", name, len);
    size_t n;
    while ((n = fread(buf, 1, sizeof buf, f)) > 0) {
        crc = esp_rom_crc32_le(crc, buf, n);
        for (size_t i = 0; i < n; i++) printf("%02X", buf[i]);
        printf("\n");
    }
    printf("FILEDUMP END %08lX\n", (unsigned long)crc);
    fflush(stdout);
    usb_out_unlock();
    fclose(f);
}

void app_main(void)
{
    logbuf_init();
    usb_fwd_init();
    esp_err_t e = nvs_flash_init();
    if (e == ESP_ERR_NVS_NO_FREE_PAGES || e == ESP_ERR_NVS_NEW_VERSION_FOUND) { nvs_flash_erase(); nvs_flash_init(); }
    rlog("Kymopsis Radar Base firmware v" RB_FW_VERSION);
    radar_cfg_init();
    storage_mount();
    radar_init();
    wifi_start();
    mqtt_ha_start();
    web_start();
    xTaskCreate(console_task, "console", 4096, NULL, 3, NULL);
}
