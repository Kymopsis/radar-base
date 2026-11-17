#pragma once
/*
 * Wi-Fi for Radar Base (owner rule 2026-11-15: do not hijack the user's Wi-Fi):
 *  - home network first: credentials saved in NVS ("wifi"/ssid, pass), the unit joins it and answers
 *    as http://kymradar.local (mDNS) or its IP;
 *  - hotspot KymRadarBase-xxxx (192.168.4.1) only as fallback: when no network is saved, or the saved
 *    one has not connected for 30 s. It serves a captive portal (any name resolves to the unit, unknown
 *    pages redirect to the app) and switches itself off once the home network works and no phone is
 *    connected to it.
 * Credentials come from the web app (POST body, never a URL), or the USB console `wifi <ssid> <pass>`.
 */
#include <stdbool.h>
#include <stddef.h>

#define RB_HOSTNAME "kymradar"

void wifi_start(void);
/* Save credentials and (re)connect. Empty ssid = forget and fall back to the hotspot. */
void wifi_set_sta(const char *ssid, const char *pass);
/* JSON status: {"ap":{...},"sta":{...},"host":"kymradar.local"} */
void wifi_status_json(char *out, size_t cap);
/* JSON array of nearby networks (blocking scan, ~2 s). */
void wifi_scan_json(char *out, size_t cap);
bool wifi_ap_active(void);
/* Console: "wifi" (status) | "wifi <ssid> <password>" | "wifi forget" */
void wifi_command(const char *a, const char *b);
