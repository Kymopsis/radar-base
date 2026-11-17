#include "adc_format.h"
#include <string.h>

static const adc_fmt_t s_fmts[] = {
    { ADC_FMT_REAL_2CH, "real, 2 channels", 2, false },
    { ADC_FMT_CPLX_1CH, "complex I/Q, 1 channel", 1, true },
};

bool adc_parse_hdr(const uint8_t *b, uint32_t n, adc_hdr_t *h)
{
    if (n < ADC_HDR_LEN || memcmp(b, "KYA2", 4)) return false;
    h->frame = b[4] | b[5] << 8 | b[6] << 16 | (uint32_t)b[7] << 24;
    h->chunk = b[8];
    h->nchunks = b[9];
    h->chip = b[10];
    h->format = b[11];
    h->samples = b[12] < 16 ? 1u << b[12] : 0;
    h->chirps = b[13];
    h->len = b[14] | b[15] << 8;
    return true;
}

const adc_fmt_t *adc_fmt(uint8_t format)
{
    for (unsigned i = 0; i < sizeof s_fmts / sizeof s_fmts[0]; i++)
        if (s_fmts[i].format == format) return &s_fmts[i];
    return NULL;
}

const char *adc_chip_name(uint8_t chip)
{
    switch (chip) {
    case ADC_CHIP_60G_REAL: return "60 GHz radar (real, 2 ch)";
    case ADC_CHIP_77G_IQ: return "77 GHz radar (I/Q)";
    default: return "unknown chip";
    }
}

void adc_decode(uint8_t format, uint32_t word, int16_t v[ADC_MAX_CH][2])
{
    int16_t lo = (int16_t)(word & 0xFFFF), hi = (int16_t)(word >> 16);
    memset(v, 0, sizeof(int16_t) * ADC_MAX_CH * 2);
    switch (format) {
    case ADC_FMT_REAL_2CH: v[0][0] = hi; v[1][0] = lo; break;
    case ADC_FMT_CPLX_1CH: v[0][0] = lo; v[0][1] = hi; break;
    default: break;
    }
}
