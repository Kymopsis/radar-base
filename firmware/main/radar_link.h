#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "sdkconfig.h"
#define RB_DEVKIT 0   /* open build */
#define RB_FW_VERSION "0.9"   /* ESP32 Radar Base firmware */

typedef enum { BOOT_FLASH, BOOT_XMODEM, BOOT_DEBUG } boot_mode_t;

/* Loader name that selects the flash loader embedded in this firmware. */
#define BUILTIN_LOADER "builtin"

typedef enum {
    JOB_NONE, JOB_VERSION, JOB_RESET_FLASH, JOB_ROM_PROBE, JOB_RAM_RUN, JOB_FLASH, JOB_OTA_PROBE,
    JOB_BACKUP, JOB_SPI_TEST, JOB_BATCH,
} job_op_t;

void radar_init(void);
/* Queue a job for the worker task. Returns false if one is already running. */
bool radar_submit(job_op_t op, const char *file_a, const char *file_b);
bool radar_busy(void);
const char *radar_pin_summary(void);
/* Send one TinyFrame message to the radar application on its UART0 (scenario app commands). */
void radar_tf_send(uint16_t type, const uint8_t *data, uint16_t len);
/* Send the current stream configuration to the radar app if a stream is running. */
void radar_cfg_push(void);
/* Diagnostics: sample the pad level of every radar line while the boot UART sends 0x55. */
void radar_pin_probe(void);
