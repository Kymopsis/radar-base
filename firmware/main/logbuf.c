#include "logbuf.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_timer.h"
#include "usb_fwd.h"

#define LOG_LINES 256
#define LOG_LEN   160

static char s_lines[LOG_LINES][LOG_LEN];
static uint32_t s_next;          /* seq of the next line to be written */
static SemaphoreHandle_t s_mu;

void logbuf_init(void) { s_mu = xSemaphoreCreateMutex(); }

void rlog(const char *fmt, ...)
{
    char tmp[LOG_LEN];
    int n = snprintf(tmp, sizeof tmp, "%7.2f ", esp_timer_get_time() / 1e6);
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(tmp + n, sizeof tmp - n, fmt, ap);
    va_end(ap);
    usb_out_lock();          /* never split a binary frame packet on the USB console */
    printf("%s\n", tmp);
    fflush(stdout);
    usb_out_unlock();
    xSemaphoreTake(s_mu, portMAX_DELAY);
    strcpy(s_lines[s_next % LOG_LINES], tmp);
    s_next++;
    xSemaphoreGive(s_mu);
}

static size_t json_esc(char *o, size_t cap, const char *s)
{
    size_t k = 0;
    for (; *s && k + 7 < cap; s++) {
        unsigned char c = (unsigned char)*s;
        if (c == '"' || c == '\\') { o[k++] = '\\'; o[k++] = c; }
        else if (c < 0x20 || c > 0x7e) k += snprintf(o + k, cap - k, "\\u%04x", c);
        else o[k++] = c;
    }
    o[k] = 0;
    return k;
}

uint32_t logbuf_json(uint32_t since, char *out, size_t cap)
{
    xSemaphoreTake(s_mu, portMAX_DELAY);
    uint32_t first = s_next > LOG_LINES ? s_next - LOG_LINES : 0;
    if (since < first) since = first;
    size_t k = snprintf(out, cap, "{\"lines\":[");
    uint32_t i = since;
    for (; i < s_next && k + LOG_LEN * 2 < cap; i++) {
        if (i != since) out[k++] = ',';
        out[k++] = '"';
        k += json_esc(out + k, cap - k - 4, s_lines[i % LOG_LINES]);
        out[k++] = '"';
    }
    snprintf(out + k, cap - k, "],\"next\":%lu}", (unsigned long)i);
    xSemaphoreGive(s_mu);
    return i;
}
