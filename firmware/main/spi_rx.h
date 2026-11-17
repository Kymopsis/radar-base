#pragma once
#include <stdbool.h>

/* Receive the radar SPI0 stream test for `seconds` and log statistics. */
bool spi_rx_run(int sclk, int mosi, int cs, int seconds);
/* Keep one complete ADC frame of the next/current stream and print it as hex on the USB console
   ("FRAMEDUMP BEGIN len chip format samples chirps" ... "FRAMEDUMP END") when the stream ends. */
void spi_rx_request_dump(void);
/* End a running stream (seconds = 0 runs until this is called). */
void spi_rx_request_stop(void);
