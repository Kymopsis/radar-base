#pragma once
/*
 * Radar ADC frame formats streamed over the radar SPI0 link (header "KYA2").
 * The radar firmware names its chip and sample format in every chunk header, so the ESP32 side
 * decodes by table instead of assuming one chip:
 *   the 60 GHz radar samples real-valued (2 channels); the 77 GHz radar samples complex I/Q.
 *
 * Chunk header (16 bytes):
 *   0..3 'K','Y','A','2' | 4..7 frame u32 LE | 8 chunk | 9 nchunks | 10 chip id | 11 format
 *   12 log2(samples per chirp) | 13 chirps | 14..15 payload length u16 LE
 * Payload: 32-bit little-endian words; how the two int16 halves map to channels is the format.
 */
#include <stdbool.h>
#include <stdint.h>

#define ADC_HDR_LEN        16
#define ADC_CHIP_60G_REAL  0x61   /* 60 GHz, real sampling */
#define ADC_CHIP_77G_IQ    0x31   /* 77 GHz, complex sampling */

#define ADC_FMT_REAL_2CH   1      /* word = {ch0 real [31:16], ch1 real [15:0]} */
#define ADC_FMT_CPLX_1CH   2      /* word = {ch0 Q [31:16], ch0 I [15:0]} */

#define ADC_MAX_CH         2

typedef struct {
    uint32_t frame;
    uint8_t chunk, nchunks, chip, format, chirps;
    uint32_t samples;      /* per chirp */
    uint32_t len;          /* payload bytes in this chunk */
} adc_hdr_t;

typedef struct {
    uint8_t format;
    const char *name;
    uint8_t channels;
    bool complex;          /* true: each channel has I and Q; false: one real value */
} adc_fmt_t;

/* Parse a chunk header; false if it is not a KYA2 header. */
bool adc_parse_hdr(const uint8_t *b, uint32_t n, adc_hdr_t *h);
/* Format description, or NULL if unknown. */
const adc_fmt_t *adc_fmt(uint8_t format);
const char *adc_chip_name(uint8_t chip);
/* Decode one 32-bit word: v[ch][0] = real or I, v[ch][1] = Q (complex formats only). */
void adc_decode(uint8_t format, uint32_t word, int16_t v[ADC_MAX_CH][2]);
