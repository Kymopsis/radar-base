#pragma once
/*
 * Forward complete ADC frames to the PC over the native USB port (same port as the console).
 * Packet: 'K','Y','F','2' | frame u32 | chip u8 | format u8 | samples u16 | chirps u16 | rsv u16 |
 *         payload len u32 | payload | CRC-32 (zlib) u32 over the payload. All little endian.
 * The newest complete frame is sent whenever the USB link is free; frames arriving meanwhile are
 * skipped, so the radar stream never stalls. Console text lines keep flowing between packets.
 */
#include <stdbool.h>
#include <stdint.h>
#include "adc_format.h"

void usb_fwd_init(void);
void usb_fwd_enable(bool on);
bool usb_fwd_enabled(void);
/* Called by the SPI receiver for every good ADC chunk (in order). */
void usb_fwd_offer_chunk(const adc_hdr_t *h, const uint8_t *payload);
/* Scenario app results: every TinyFrame message from the radar app port is forwarded as
   'K','Y','T','1' | type u16 | len u16 | payload | CRC-32 u32 when "appout on". */
void usb_fwd_tf_enable(bool on);
void usb_fwd_tf(uint16_t type, const uint8_t *payload, uint16_t len);
/* Raw radar app port (UART0) bytes, exactly as a USB-UART adapter would see them, when "u0raw on":
   'K','Y','R','1' | len u16 | bytes. */
void usb_fwd_raw_enable(bool on);
void usb_fwd_raw(const uint8_t *b, int n);
/* One text line (e.g. a schema v0 JSON line, ending in a newline) on the USB console. */
void usb_fwd_text(const char *s, int n);
/* Serialises writers of the USB console (log lines vs. binary packets). */
void usb_out_lock(void);
void usb_out_unlock(void);
