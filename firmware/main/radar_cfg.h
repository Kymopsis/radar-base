#pragma once
/*
 * Radar stream configuration, edited on the ESP32 (console `cfg`), kept in NVS and pushed to the
 * radar app over the radar's UART1 as 'K','Y','C','1' + struct + CRC16-CCITT (LE).
 * Layout must match the configuration struct of the raw-data radar app: 44 bytes, naturally aligned, LE.
 * 60 GHz radar (real sampling, ADC clock 7.14 MHz which is its maximum).
 * Head/tail trim is not configurable: skip_chirps is always 1 and tail_samples always 0 (= the
 * SDK driver's default window: first chirp dropped, the T1+T2 part of each chirp cut).
 */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint32_t fstart_mhz;
    uint16_t bw_mhz, t0_us, t1_us, t2_us;
    uint16_t samples, tail_samples, rc_hpf_khz, frame_gap_ms;
    uint8_t ver, chirps, skip_chirps, downsample;
    uint8_t tx0, tx1, rx0, rx1;
    uint8_t pa0, pa1, tia0, tia1;
    uint8_t pga0, pga1, tia_hpf_en, tia_hpf;
    uint8_t rc_hpf_en, pga_hpf_en, pga_hpf, pga_lpf;
    uint8_t spi_cpsr;
    uint8_t frame_period_ms;   /* 0 = free running (+frame_gap_ms); 1..255 = timer-paced fixed frame period */
    uint8_t rsv[2];
} radar_cfg_t;

_Static_assert(sizeof(radar_cfg_t) == 44, "radar_cfg_t must match the raw-data radar app");

void radar_cfg_init(void);                 /* load from NVS or defaults */
const radar_cfg_t *radar_cfg_get(void);
/* Console: "" or "show", "reset", "<key> <value>". Logs the result. */
void radar_cfg_command(const char *key, const char *value);
/* NULL if valid, else the reason; fills the derived window values. */
const char *radar_cfg_check(const radar_cfg_t *c, uint32_t *head, uint32_t *tail, uint32_t *interval, uint32_t *bytes);
/* Encode the UART message; returns its length. */
size_t radar_cfg_encode(const radar_cfg_t *c, uint8_t *out, size_t max);
/* Current configuration as JSON (console units and keys). */
void radar_cfg_json(char *out, size_t cap);
