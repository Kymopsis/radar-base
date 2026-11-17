/*
 * SPI slave receiver for the radar SPI0 stream test.
 * The radar is SPI master (its own PL022 on the flash bus, flash kept deselected); the ESP32
 * receives with a hardware SPI slave + DMA. Chip select comes from radar P12 (a free GPIO after
 * boot). Each CS-low transaction is one chunk: 16-byte header + payload (see the radar test
 * program for the format). Statistics are logged once per second and as a summary.
 */
#include "spi_rx.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "driver/spi_slave.h"
#include "driver/gpio.h"
#include "esp_heap_caps.h"
#include "esp_timer.h"
#include "logbuf.h"
#include "adc_format.h"
#include "usb_fwd.h"

#define HOST     SPI2_HOST
#define CHUNK    16384
#define HDR      16
#define BUFSZ    (HDR + CHUNK + 64)
#define NBUF     4

static volatile bool s_dump_req, s_stop_req;
void spi_rx_request_dump(void) { s_dump_req = true; }
void spi_rx_request_stop(void) { s_stop_req = true; }

static uint32_t rd32(const uint8_t *b) { return b[0] | b[1] << 8 | b[2] << 16 | (uint32_t)b[3] << 24; }

bool spi_rx_run(int sclk, int mosi, int cs, int seconds)
{
    spi_bus_config_t bus = {
        .mosi_io_num = mosi, .miso_io_num = -1, .sclk_io_num = sclk,
        .quadwp_io_num = -1, .quadhd_io_num = -1, .max_transfer_sz = BUFSZ,
    };
    spi_slave_interface_config_t sc = { .mode = 0, .spics_io_num = cs, .queue_size = NBUF };
    gpio_set_pull_mode(cs, GPIO_PULLUP_ONLY);
    if (spi_slave_initialize(HOST, &bus, &sc, SPI_DMA_CH_AUTO) != ESP_OK) {
        rlog("spi rx: slave init failed");
        return false;
    }
    uint8_t *ref = malloc(CHUNK);
    uint8_t *buf[NBUF] = {0};
    spi_slave_transaction_t tr[NBUF];
    bool ok = ref != NULL;
    for (int i = 0; i < NBUF && ok; i++) {
        buf[i] = heap_caps_malloc(BUFSZ, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
        ok = buf[i] != NULL;
    }
    if (!ok) { rlog("spi rx: out of DMA memory"); goto out; }
    for (uint32_t k = 0; k < CHUNK; k++) ref[k] = (uint8_t)(k * 13u + 7u + (k >> 8));
    for (int i = 0; i < NBUF; i++) {
        memset(&tr[i], 0, sizeof tr[i]);
        tr[i].length = BUFSZ * 8;
        tr[i].rx_buffer = buf[i];
        spi_slave_queue_trans(HOST, &tr[i], portMAX_DELAY);
    }
    if (seconds > 0) rlog("spi rx: armed (SCLK GPIO%d, data GPIO%d, CS GPIO%d), listening %d s", sclk, mosi, cs, seconds);
    else rlog("spi rx: armed (SCLK GPIO%d, data GPIO%d, CS GPIO%d), running until 'stop'", sclk, mosi, cs);

    uint32_t ok_n = 0, bad_hdr = 0, bad_len = 0, bad_data = 0, missing = 0, sec_ok = 0, sec_bytes = 0;
    uint32_t best_kbs = 0, exp_frame = 0, exp_chunk = 0, first_frame = 0;
    bool have = false;
    uint32_t adc_frames = 0, st_n = 0;
    adc_hdr_t ah0 = {0};                       /* header of the first ADC chunk seen */
    int64_t st_sum[ADC_MAX_CH][2] = {{0}};
    int32_t st_min = 32767, st_max = -32768;
    uint8_t *dump_buf = NULL;
    uint32_t dump_cap = 0, dump_len = 0, dump_frame = 0;
    adc_hdr_t dump_hdr = {0};
    bool dump_done = false;
    int64_t t_start = esp_timer_get_time();
    int64_t t_end = esp_timer_get_time() + (int64_t)seconds * 1000000, t_sec = esp_timer_get_time();
    s_stop_req = false;
    while ((seconds <= 0 || esp_timer_get_time() < t_end) && !s_stop_req) {
        spi_slave_transaction_t *r;
        if (spi_slave_get_trans_result(HOST, &r, pdMS_TO_TICKS(100)) == ESP_OK) {
            const uint8_t *b = r->rx_buffer;
            uint32_t n = r->trans_len / 8;
            adc_hdr_t ah;
            bool adc = adc_parse_hdr(b, n, &ah);
            if (n == 0) {
                /* empty CS pulse, ignore */
            } else if (adc) {
                const adc_fmt_t *f = adc_fmt(ah.format);
                if (n != HDR + ah.len || !f) bad_len++;
                else {
                    ok_n++; sec_ok++; sec_bytes += n;
                    if (!have) {
                        have = true; first_frame = ah.frame; ah0 = ah;
                        rlog("adc: %s, %s, %lu samples x %u chirps per frame, %u chunks", adc_chip_name(ah.chip),
                             f->name, (unsigned long)ah.samples, ah.chirps, ah.nchunks);
                    } else if (ah.frame != exp_frame || ah.chunk != exp_chunk) {
                        int64_t lost = ((int64_t)ah.frame * ah.nchunks + ah.chunk) - ((int64_t)exp_frame * ah.nchunks + exp_chunk);
                        if (lost > 0) missing += (uint32_t)lost;
                    }
                    usb_fwd_offer_chunk(&ah, b + HDR);
                    /* frame dump: keep one complete frame, taken ~2 s into the stream */
                    if (s_dump_req && !dump_done && esp_timer_get_time() - t_start > 2000000) {
                        if (!dump_buf) {
                            dump_cap = (uint32_t)ah.nchunks * CHUNK;
                            dump_buf = heap_caps_malloc(dump_cap, MALLOC_CAP_SPIRAM);
                        }
                        if (dump_buf) {
                            if (ah.chunk == 0) { dump_frame = ah.frame; dump_len = 0; dump_hdr = ah; }
                            if (ah.frame == dump_frame && ah.chunk * CHUNK == dump_len && dump_len + ah.len <= dump_cap) {
                                memcpy(dump_buf + dump_len, b + HDR, ah.len);
                                dump_len += ah.len;
                                if (ah.chunk + 1 == ah.nchunks) dump_done = true;
                            }
                        }
                    }
                    exp_chunk = ah.chunk + 1; exp_frame = ah.frame;
                    if (exp_chunk >= ah.nchunks) { exp_chunk = 0; exp_frame++; adc_frames++; }
                    if (ah.chunk == 0) {   /* statistics of the first chunk of each frame */
                        const uint32_t *w = (const uint32_t *)(b + HDR);
                        int16_t v[ADC_MAX_CH][2];
                        for (uint32_t k = 0; k < ah.len / 4; k++) {
                            adc_decode(ah.format, w[k], v);
                            for (int c = 0; c < f->channels; c++)
                                for (int p = 0; p < (f->complex ? 2 : 1); p++) {
                                    int32_t x = v[c][p];
                                    st_sum[c][p] += (int64_t)x * x;
                                    if (x < st_min) st_min = x;
                                    if (x > st_max) st_max = x;
                                }
                            st_n++;
                        }
                    }
                }
            } else if (n < HDR || memcmp(b, "KYS1", 4)) {
                if (bad_hdr < 3) {
                    char hex[100] = {0};
                    for (int i = 0; i < 24 && i < (int)n; i++) sprintf(hex + i * 3, "%02X ", b[i]);
                    rlog("spi rx: bad chunk, %lu bytes, starts %s", (unsigned long)n, hex);
                }
                bad_hdr++;
            } else {
                uint32_t frame = rd32(b + 4), chunk = b[8], nch = b[10], len = rd32(b + 12);
                if (len != CHUNK || n != HDR + len) bad_len++;
                else if (memcmp(b + HDR, ref, CHUNK)) bad_data++;
                else {
                    ok_n++; sec_ok++; sec_bytes += n;
                    if (!have) { have = true; first_frame = frame; }
                    else if (frame != exp_frame || chunk != exp_chunk) {
                        int64_t lost = ((int64_t)frame * nch + chunk) - ((int64_t)exp_frame * nch + exp_chunk);
                        if (lost > 0) missing += (uint32_t)lost;
                    }
                    exp_chunk = chunk + 1; exp_frame = frame;
                    if (exp_chunk >= nch) { exp_chunk = 0; exp_frame++; }
                }
            }
            r->trans_len = 0;
            spi_slave_queue_trans(HOST, r, portMAX_DELAY);
        }
        int64_t now = esp_timer_get_time();
        if (now - t_sec >= 1000000) {
            uint32_t kbs = (uint32_t)((uint64_t)sec_bytes * 1000000 / 1024 / (now - t_sec));
            if (sec_ok) rlog("spi rx: %lu chunks ok in the last second, %lu KB/s (total ok %lu, bad hdr %lu len %lu data %lu, missing %lu)",
                             (unsigned long)sec_ok, (unsigned long)kbs, (unsigned long)ok_n, (unsigned long)bad_hdr,
                             (unsigned long)bad_len, (unsigned long)bad_data, (unsigned long)missing);
            const adc_fmt_t *f0 = adc_fmt(ah0.format);
            if (st_n && f0) {
                char s[160];
                int k = 0;
                for (int c = 0; c < f0->channels; c++) {
                    if (f0->complex)
                        k += snprintf(s + k, sizeof s - k, " ch%d rms I %.0f Q %.0f", c,
                                      sqrt((double)st_sum[c][0] / st_n), sqrt((double)st_sum[c][1] / st_n));
                    else
                        k += snprintf(s + k, sizeof s - k, " ch%d rms %.0f", c, sqrt((double)st_sum[c][0] / st_n));
                }
                rlog("adc: %lu frames,%s, min %ld max %ld (chunk 0 of each frame)",
                     (unsigned long)adc_frames, s, (long)st_min, (long)st_max);
                st_n = 0; memset(st_sum, 0, sizeof st_sum); st_min = 32767; st_max = -32768;
            }
            if (kbs > best_kbs) best_kbs = kbs;
            sec_ok = sec_bytes = 0;
            t_sec = now;
        }
    }
    if (adc_frames) rlog("RESULT: adc stream %lu complete frames (%s)", (unsigned long)adc_frames, adc_chip_name(ah0.chip));
    if (s_dump_req) {
        s_dump_req = false;
        if (dump_done) {
            /* raw frame as hex on the USB console, for analysis on a PC */
            rlog("dump: frame %lu, %lu bytes, chip 0x%02X format %u, %lu samples x %u chirps",
                 (unsigned long)dump_frame, (unsigned long)dump_len, dump_hdr.chip, dump_hdr.format,
                 (unsigned long)dump_hdr.samples, dump_hdr.chirps);
            vTaskDelay(pdMS_TO_TICKS(200));
            printf("\nFRAMEDUMP BEGIN %lu %u %u %lu %u\n", (unsigned long)dump_len, dump_hdr.chip, dump_hdr.format,
                   (unsigned long)dump_hdr.samples, dump_hdr.chirps);
            char line[2 * 64 + 2];
            for (uint32_t i = 0; i < dump_len; i += 64) {
                int k = 0;
                for (uint32_t j = i; j < i + 64 && j < dump_len; j++) k += sprintf(line + k, "%02X", dump_buf[j]);
                line[k++] = '\n'; line[k] = 0;
                fputs(line, stdout);
                if ((i & 0xFFF) == 0) { fflush(stdout); vTaskDelay(1); }
            }
            printf("FRAMEDUMP END\n");
            fflush(stdout);
        } else rlog("dump: no complete frame captured");
    }
    heap_caps_free(dump_buf);
    rlog("RESULT: spi rx %lu chunks ok from frame %lu, bad hdr %lu, bad len %lu, bad data %lu, missing %lu, best %lu KB/s",
         (unsigned long)ok_n, (unsigned long)first_frame, (unsigned long)bad_hdr,
         (unsigned long)bad_len, (unsigned long)bad_data, (unsigned long)missing, (unsigned long)best_kbs);
out:
    spi_slave_free(HOST);
    for (int i = 0; i < NBUF; i++) heap_caps_free(buf[i]);
    free(ref);
    return ok;
}
