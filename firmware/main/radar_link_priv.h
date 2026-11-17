#pragma once
/*
 * Internal radar-link helpers shared with optional modules.
 * Not a public API.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "driver/uart.h"
#include "radar_link.h"

#define U_DATA UART_NUM_1
#define U_BOOT UART_NUM_2
#define RAM_IMAGE_MAX 62976

int64_t now_ms(void);
bool wait_for_C(uart_port_t u, const char *tag, int timeout_ms);
bool wait_for_text(uart_port_t u, const char *tag, const char *needle, int timeout_ms);
uart_port_t wait_loader_C(int timeout_ms);
bool xmodem_send(uart_port_t u, const uint8_t *data, size_t len, bool allow_retry, const char *tag);
bool has_header(const uint8_t *b, size_t len, uint32_t *code_len, uint32_t *code_off);
uint16_t crc16(const uint8_t *p, int n);
uint8_t *load_file(const char *name, size_t *len);   /* a file from /bins (malloc'd) */
void set_boot(boot_mode_t m);
void release_boot_pins(void);
void pulse_reset(void);
bool query_version(void);
bool enter_rom_xmodem(void);
void radar_lock_uarts(void);
void radar_unlock_uarts(void);
