#include "radar_cfg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "nvs.h"
#include "logbuf.h"
#include "radar_link.h"

#define NVS_NS  "radarcfg"
#define NVS_KEY "cfg1"

/* Defaults: reference waveform, own TX0 on, full 7.14 MHz ADC rate, SDK head/tail trim. */
static const radar_cfg_t s_default = {
    .fstart_mhz = 58500, .bw_mhz = 2500, .t0_us = 672, .t1_us = 14, .t2_us = 0,
    .samples = 4096, .tail_samples = 0, .rc_hpf_khz = 150, .frame_gap_ms = 0,
    .ver = 1, .chirps = 8, .skip_chirps = 1, .downsample = 0,
    .tx0 = 1, .tx1 = 0, .rx0 = 1, .rx1 = 1,
    .pa0 = 10, .pa1 = 0, .tia0 = 3, .tia1 = 3,
    .pga0 = 13, .pga1 = 13, .tia_hpf_en = 1, .tia_hpf = 0,   /* 26 dB: 38 dB nearly clips with TX0 on */
    .rc_hpf_en = 1, .pga_hpf_en = 1, .pga_hpf = 0, .pga_lpf = 1,
    .spi_cpsr = 4,
};

static radar_cfg_t s_cfg;

static void save(void)
{
    nvs_handle_t h;
    if (nvs_open(NVS_NS, NVS_READWRITE, &h) != ESP_OK) return;
    nvs_set_blob(h, NVS_KEY, &s_cfg, sizeof s_cfg);
    nvs_commit(h);
    nvs_close(h);
}

void radar_cfg_init(void)
{
    s_cfg = s_default;
    nvs_handle_t h;
    if (nvs_open(NVS_NS, NVS_READONLY, &h) == ESP_OK) {
        radar_cfg_t c;
        size_t n = sizeof c;
        uint32_t a, b, d, e;
        if (nvs_get_blob(h, NVS_KEY, &c, &n) == ESP_OK && n == sizeof c) {
            c.skip_chirps = 1;      /* head/tail trim is the SDK's fixed default, not a setting */
            c.tail_samples = 0;
            if (!radar_cfg_check(&c, &a, &b, &d, &e)) s_cfg = c;
        }
        nvs_close(h);
    }
}

const radar_cfg_t *radar_cfg_get(void) { return &s_cfg; }

const char *radar_cfg_check(const radar_cfg_t *c, uint32_t *head, uint32_t *tail, uint32_t *interval, uint32_t *bytes)
{
    if (c->ver != 1) return "unknown config version";
    if (!c->bw_mhz || c->bw_mhz > 4000 || c->fstart_mhz < 57000 || c->fstart_mhz + c->bw_mhz > 64000) return "sweep outside 57-64 GHz";
    if (c->t0_us < 10 || c->t0_us > 4000 || c->t1_us < 2 || c->t1_us > 1000 || c->t2_us > 1000) return "chirp times out of range";
    if (c->chirps < 1 || c->chirps > 128 || c->skip_chirps < 1 || c->skip_chirps > 8) return "chirp count out of range";
    if (c->samples < 16 || c->samples > 4096 || (c->samples & (c->samples - 1))) return "samples must be a power of 2, 16..4096";
    if (c->downsample > 15) return "downsample 0..15";
    if (c->tia0 > 3 || c->tia1 > 3 || c->pga0 > 20 || c->pga1 > 20 || c->pa0 > 15 || c->pa1 > 15) return "gain out of range";
    if (c->tia_hpf > 1 || (c->rc_hpf_khz != 150 && c->rc_hpf_khz != 400) || c->pga_hpf > 1 || c->pga_lpf > 3) return "filter setting out of range";
    if (c->spi_cpsr < 2 || (c->spi_cpsr & 1)) return "SPI divider must be even and >= 2";
    if (!(c->rx0 || c->rx1)) return "no receiver enabled";
    /* ADC clock 7.14 MHz = 140 ns, same arithmetic as the radar SDK */
    *interval = (uint32_t)floor((c->t0_us + c->t1_us + c->t2_us) * 1000.0 / 140.0);
    *tail = c->tail_samples ? c->tail_samples : (uint32_t)ceil((c->t1_us + c->t2_us) * (100.0 / 14.0));
    uint32_t valid = (uint32_t)c->samples * (c->downsample + 1u);
    if (valid + *tail > *interval) return "samples x (downsample+1) + tail longer than the chirp";
    *head = *interval - valid - *tail;
    *bytes = (uint32_t)(c->chirps + c->skip_chirps) * c->samples * 4u;
    if (*bytes > 0x28000u) return "frame larger than radar RAM (160 KB incl. dropped chirps)";
    return NULL;
}

static uint16_t crc16(const uint8_t *p, size_t n)
{
    uint16_t c = 0;
    while (n--) {
        c ^= (uint16_t)(*p++) << 8;
        for (int i = 0; i < 8; i++) c = (c & 0x8000) ? (uint16_t)((c << 1) ^ 0x1021) : (uint16_t)(c << 1);
    }
    return c;
}

size_t radar_cfg_encode(const radar_cfg_t *c, uint8_t *out, size_t max)
{
    size_t n = 4 + sizeof *c + 2;
    if (max < n) return 0;
    memcpy(out, "KYC1", 4);
    memcpy(out + 4, c, sizeof *c);
    uint16_t k = crc16(out, 4 + sizeof *c);
    out[n - 2] = (uint8_t)k;
    out[n - 1] = (uint8_t)(k >> 8);
    return n;
}

static void show(const radar_cfg_t *c)
{
    static const char *lpf[] = { "2", "4", "6", "8" };
    uint32_t head = 0, tail = 0, interval = 0, bytes = 0;
    const char *err = radar_cfg_check(c, &head, &tail, &interval, &bytes);
    rlog("cfg: fstart %lu MHz, bw %u MHz (%lu-%lu MHz), t0 %u t1 %u t2 %u us, gap %u ms, period %u ms%s",
         (unsigned long)c->fstart_mhz, c->bw_mhz, (unsigned long)c->fstart_mhz,
         (unsigned long)(c->fstart_mhz + c->bw_mhz), c->t0_us, c->t1_us, c->t2_us, c->frame_gap_ms,
         c->frame_period_ms, c->frame_period_ms ? " (timer paced)" : " (free running)");
    rlog("cfg: samples %u, chirps %u, ds %u (ADC 7.14 MHz / %u); trim fixed by the SDK: first chirp dropped, T1+T2 cut",
         c->samples, c->chirps, c->downsample, c->downsample + 1);
    rlog("cfg: tx0 %u tx1 %u rx0 %u rx1 %u, pa0 %u pa1 %u, tia0 %u dB tia1 %u dB, pga0 %u dB pga1 %u dB",
         c->tx0, c->tx1, c->rx0, c->rx1, c->pa0, c->pa1, c->tia0 * 6, c->tia1 * 6, c->pga0 * 2, c->pga1 * 2);
    rlog("cfg: tiahpf %s, rchpf %s, pgahpf %s, pgalpf %s MHz, spi %.2f MHz",
         c->tia_hpf_en ? (c->tia_hpf ? "200 kHz" : "100 kHz") : "off",
         c->rc_hpf_en ? (c->rc_hpf_khz == 400 ? "400 kHz" : "150 kHz") : "off",
         c->pga_hpf_en ? (c->pga_hpf ? "500 kHz" : "100 kHz") : "off",
         lpf[c->pga_lpf & 3], 100.0 / c->spi_cpsr);
    if (err) rlog("cfg: INVALID: %s", err);
    else rlog("cfg: window per chirp %lu ADC clocks = head %lu + %lu valid + tail %lu; %lu bytes sent per frame",
              (unsigned long)interval, (unsigned long)head, (unsigned long)(interval - head - tail),
              (unsigned long)tail, (unsigned long)((uint32_t)c->chirps * c->samples * 4u));
}

/* Parse "off" / numbers for the filter keys. Returns false on an unknown value. */
static bool set_filter(const char *v, uint8_t *en, uint8_t *sel, int a, int b)
{
    if (!strcmp(v, "off")) { *en = 0; return true; }
    int x = atoi(v);
    if (x == a) { *en = 1; *sel = 0; return true; }
    if (x == b) { *en = 1; *sel = 1; return true; }
    return false;
}

void radar_cfg_command(const char *key, const char *val)
{
    if (!key[0] || !strcmp(key, "show")) { show(&s_cfg); return; }
    if (!strcmp(key, "reset")) { s_cfg = s_default; save(); rlog("cfg: defaults restored"); show(&s_cfg); radar_cfg_push(); return; }
    if (!strcmp(key, "send")) { radar_cfg_push(); return; }
    if (!strcmp(key, "preset")) {
        /* vital: slow-time capture for breathing / heartbeat / HRV. One chirp of 256 samples every
           5 ms (200 Hz, 1 KB per frame = 200 KB/s, fits USB). T0 60 us: 256 samples span ~36 us
           = ~1.5 GHz swept = ~10 cm range bins, max range ~12 m. */
        radar_cfg_t c = s_default;
        /* PGA 14 dB: with the short chirp, 26 dB clips (measured peak 11k of 32k at 14 dB) */
        if (!strcmp(val, "vital")) { c.samples = 256; c.chirps = 1; c.t0_us = 60; c.frame_period_ms = 5; c.pga0 = c.pga1 = 7; }
        else if (strcmp(val, "default")) { rlog("cfg preset: vital | default"); return; }
        s_cfg = c; save(); rlog("cfg: preset %s", val); show(&s_cfg); radar_cfg_push(); return;
    }
    if (!val[0]) {
        rlog("cfg keys: fstart bw t0 t1 t2 (MHz/us) | samples chirps ds gap period (ms, 0 = free running) | tx0 tx1 rx0 rx1 | pa pa0 pa1 (0-15) | "
             "tia tia0 tia1 (dB 0/6/12/18) | pga pga0 pga1 (dB 0-40 even) | tiahpf 100/200/off | rchpf 150/400/off | "
             "pgahpf 100/500/off | pgalpf 2/4/6/8 | spi (MHz: 50 25 12.5 ...) ; cfg show | reset | send | preset vital/default");
        return;
    }
    radar_cfg_t c = s_cfg;
    long v = strtol(val, NULL, 10);
    bool ok = true;
    if (!strcmp(key, "fstart")) c.fstart_mhz = v;
    else if (!strcmp(key, "bw")) c.bw_mhz = v;
    else if (!strcmp(key, "t0")) c.t0_us = v;
    else if (!strcmp(key, "t1")) c.t1_us = v;
    else if (!strcmp(key, "t2")) c.t2_us = v;
    else if (!strcmp(key, "samples")) c.samples = v;
    else if (!strcmp(key, "chirps")) c.chirps = v;
    else if (!strcmp(key, "ds")) c.downsample = v;
    else if (!strcmp(key, "gap")) c.frame_gap_ms = v;
    else if (!strcmp(key, "period")) { ok = v >= 0 && v <= 255; c.frame_period_ms = v; }
    else if (!strcmp(key, "tx0")) c.tx0 = !!v;
    else if (!strcmp(key, "tx1")) c.tx1 = !!v;
    else if (!strcmp(key, "rx0")) c.rx0 = !!v;
    else if (!strcmp(key, "rx1")) c.rx1 = !!v;
    else if (!strcmp(key, "pa")) c.pa0 = c.pa1 = v;
    else if (!strcmp(key, "pa0")) c.pa0 = v;
    else if (!strcmp(key, "pa1")) c.pa1 = v;
    else if (!strcmp(key, "tia") || !strcmp(key, "tia0") || !strcmp(key, "tia1")) {
        ok = v >= 0 && v <= 18 && v % 6 == 0;
        if (key[3] != '1') c.tia0 = v / 6;
        if (key[3] != '0') c.tia1 = v / 6;
    } else if (!strcmp(key, "pga") || !strcmp(key, "pga0") || !strcmp(key, "pga1")) {
        ok = v >= 0 && v <= 40 && v % 2 == 0;
        if (key[3] != '1') c.pga0 = v / 2;
        if (key[3] != '0') c.pga1 = v / 2;
    } else if (!strcmp(key, "tiahpf")) ok = set_filter(val, &c.tia_hpf_en, &c.tia_hpf, 100, 200);
    else if (!strcmp(key, "rchpf")) {
        if (!strcmp(val, "off")) c.rc_hpf_en = 0;
        else if (v == 150 || v == 400) { c.rc_hpf_en = 1; c.rc_hpf_khz = v; }
        else ok = false;
    } else if (!strcmp(key, "pgahpf")) ok = set_filter(val, &c.pga_hpf_en, &c.pga_hpf, 100, 500);
    else if (!strcmp(key, "pgalpf")) { ok = v == 2 || v == 4 || v == 6 || v == 8; c.pga_lpf = v / 2 - 1; }
    else if (!strcmp(key, "spi")) {
        double mhz = atof(val);
        int cpsr = mhz > 0 ? (int)lround(100.0 / mhz) : 0;
        if (cpsr & 1) cpsr++;
        ok = cpsr >= 2 && cpsr <= 254;
        c.spi_cpsr = cpsr;
    } else { rlog("cfg: unknown key '%s' (type cfg ? for the list)", key); return; }

    uint32_t head, tail, interval, bytes;
    const char *err = ok ? radar_cfg_check(&c, &head, &tail, &interval, &bytes) : "value not allowed for this key";
    if (err) { rlog("cfg: %s %s rejected: %s", key, val, err); return; }
    s_cfg = c;
    save();
    show(&s_cfg);
    radar_cfg_push();
}

/* Current configuration as JSON, in the same units and keys as the console (`cfg <key> <value>`). */
void radar_cfg_json(char *out, size_t cap)
{
    const radar_cfg_t *c = &s_cfg;
    static const char *lpf[] = { "2", "4", "6", "8" };
    uint32_t head = 0, tail = 0, interval = 0, bytes = 0;
    const char *err = radar_cfg_check(c, &head, &tail, &interval, &bytes);
    snprintf(out, cap,
             "{\"fstart\":%lu,\"bw\":%u,\"t0\":%u,\"t1\":%u,\"t2\":%u,\"samples\":%u,\"chirps\":%u,\"ds\":%u,"
             "\"gap\":%u,\"period\":%u,\"tx0\":%u,\"tx1\":%u,\"rx0\":%u,\"rx1\":%u,\"pa0\":%u,\"pa1\":%u,"
             "\"tia0\":%u,\"tia1\":%u,\"pga0\":%u,\"pga1\":%u,\"tiahpf\":\"%s\",\"rchpf\":\"%s\",\"pgahpf\":\"%s\","
             "\"pgalpf\":%s,\"spi\":%.2f,\"bytes_per_frame\":%lu,\"valid\":%s,\"error\":\"%s\"}",
             (unsigned long)c->fstart_mhz, c->bw_mhz, c->t0_us, c->t1_us, c->t2_us, c->samples, c->chirps, c->downsample,
             c->frame_gap_ms, c->frame_period_ms, c->tx0, c->tx1, c->rx0, c->rx1, c->pa0, c->pa1,
             c->tia0 * 6, c->tia1 * 6, c->pga0 * 2, c->pga1 * 2,
             c->tia_hpf_en ? (c->tia_hpf ? "200" : "100") : "off",
             c->rc_hpf_en ? (c->rc_hpf_khz == 400 ? "400" : "150") : "off",
             c->pga_hpf_en ? (c->pga_hpf ? "500" : "100") : "off",
             lpf[c->pga_lpf & 3], 100.0 / c->spi_cpsr, (unsigned long)((uint32_t)c->chirps * c->samples * 4u),
             err ? "false" : "true", err ? err : "");
}
