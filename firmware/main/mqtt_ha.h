#pragma once
/*
 * MQTT + Home Assistant discovery (open firmware). Uses the user's own broker (e.g. the Home Assistant
 * Mosquitto add-on); Kymopsis runs no broker and no cloud. Settings in NVS "mqtt": host, port, user, pass, on.
 *
 * Topics (id = last 3 bytes of the MAC, e.g. kymradar/a1f71b):
 *   <base>/status                  online / offline (retained, last will)
 *   <base>/state                   JSON: scenario, mount, occupancy, people, nearest_m, area1..4, fall, br, hr, dist_m
 *   <base>/mount/set, <base>/restart/set   commands from Home Assistant
 *   homeassistant/<component>/kymradar_<id>/<object>/config   discovery (retained)
 */
#include <stddef.h>

void mqtt_ha_start(void);                 /* reads settings; connects when the home Wi-Fi is up */
void mqtt_ha_set(const char *host, int port, const char *user, const char *pass, int on);   /* save + reconnect */
void mqtt_ha_status_json(char *out, size_t cap);   /* never contains the password */
/* Console: "mqtt" | "mqtt <host> [port] [user] [pass]" | "mqtt off" | "mqtt on" | "mqtt dump" (print the
   discovery configs and current state as "@mqtt <topic> <payload>" lines, for testing without a network) */
void mqtt_ha_command(const char *a, const char *b, const char *c, const char *d);
