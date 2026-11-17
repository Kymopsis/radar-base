#pragma once
#include <stddef.h>
#include <stdint.h>
/* Small ring of text lines shared by the console and the web page. */
void logbuf_init(void);
void rlog(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
/* Copy lines with seq >= since into out (JSON array of strings). Returns next seq. */
uint32_t logbuf_json(uint32_t since, char *out, size_t cap);
