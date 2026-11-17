#pragma once
/*
 * Radar Base JSON schema v0 (see pc-gui/SCHEMA.md): the ESP32 translates the scenario firmware's
 * TinyFrame results (3D presence, fall, vital signs) into one JSON line per message, so the PC view,
 * the ESP32 web page and later smart-home entities share one contract.
 *
 * Sinks: USB console ("jsonout on", plain text lines between the log lines) and any registered sink
 * (the web server's WebSocket). Lines are only built while at least one sink wants them.
 */
#include <stdbool.h>
#include <stdint.h>

/* Every TinyFrame message from the radar application port (called by the UART0 monitor task). */
void app_json_on_tf(uint16_t type, const uint8_t *p, uint16_t len);
/* Every command sent to the radar application (to follow mount changes immediately). */
void app_json_on_cmd(uint16_t type, const uint8_t *p, uint16_t len);
/* USB console sink. */
void app_json_usb_enable(bool on);
/* Extra sink, e.g. WebSocket broadcast. active() says whether it has listeners right now. */
typedef struct {
    bool (*active)(void);
    void (*send)(const char *line, int len);
} app_json_sink_t;
void app_json_set_sink(const app_json_sink_t *sink);
/* Re-announce the current scenario (cap) to the sinks, e.g. when a new web client connects. */
void app_json_announce(void);

/* Structured results for other integrations (MQTT / Home Assistant). Called on the radar UART task. */
typedef struct {
    bool (*active)(void);
    void (*cap)(const char *app, const char *mount);
    void (*targets)(int count, float nearest_m);
    void (*zones)(const bool occupied[4]);
    void (*fall)(const char *state);                 /* "none" | "normal" | "fall" */
    void (*vital)(float br, float hr, float dist_m); /* NAN when not measured */
} app_obs_t;
void app_json_set_observer(const app_obs_t *obs);
