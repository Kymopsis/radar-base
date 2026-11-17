/* Wi-Fi: home network first, hotspot + captive portal as fallback, mDNS name. See wifi.h. */
#include "wifi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_mac.h"
#include "nvs.h"
#include "lwip/sockets.h"
#include "mdns.h"
#include "logbuf.h"
#include "sdkconfig.h"

#define FALLBACK_AFTER_MS 30000

static esp_netif_t *s_sta, *s_ap;
static char s_ssid[33], s_pass[65], s_ap_ssid[33], s_ip[16];
static volatile bool s_connected, s_ap_on, s_has_cfg;
static volatile int s_ap_clients;
static int64_t s_down_since;
static volatile int s_last_reason;

static int64_t ms(void) { return esp_timer_get_time() / 1000; }

/* ---------------- captive-portal DNS: every A query -> 192.168.4.1 (only while the hotspot is on) ---------------- */
static void dns_task(void *arg)
{
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in a = { .sin_family = AF_INET, .sin_port = htons(53), .sin_addr.s_addr = htonl(INADDR_ANY) };
    if (s < 0 || bind(s, (struct sockaddr *)&a, sizeof a)) { rlog("wifi: captive DNS could not start"); vTaskDelete(NULL); }
    uint8_t b[512];
    for (;;) {
        struct sockaddr_in from;
        socklen_t fl = sizeof from;
        int n = recvfrom(s, b, sizeof b - 16, 0, (struct sockaddr *)&from, &fl);
        if (n < 12 || !s_ap_on) continue;
        int q = 12;                                      /* skip the question name */
        while (q < n && b[q]) q += b[q] + 1;
        q += 5;                                          /* zero byte + type + class */
        if (q > n) continue;
        b[2] = 0x81; b[3] = 0x80;                        /* response, recursion available, no error */
        b[6] = 0; b[7] = 1; b[8] = b[9] = b[10] = b[11] = 0;
        uint8_t ans[16] = { 0xC0, 0x0C, 0, 1, 0, 1, 0, 0, 0, 60, 0, 4, 192, 168, 4, 1 };
        memcpy(b + q, ans, sizeof ans);
        sendto(s, b, q + sizeof ans, 0, (struct sockaddr *)&from, fl);
    }
}

static void ap_enable(bool on)
{
    if (on == s_ap_on) return;
    esp_wifi_set_mode(on ? WIFI_MODE_APSTA : WIFI_MODE_STA);
    s_ap_on = on;
    rlog(on ? "wifi: hotspot %s on (password %s) -> http://192.168.4.1" : "wifi: hotspot %s off (home network works)",
         s_ap_ssid, CONFIG_RB_AP_PASSWORD);
}

static void sta_connect(void)
{
    wifi_config_t w = { 0 };
    strlcpy((char *)w.sta.ssid, s_ssid, sizeof w.sta.ssid);
    strlcpy((char *)w.sta.password, s_pass, sizeof w.sta.password);
    w.sta.threshold.authmode = s_pass[0] ? WIFI_AUTH_WPA_PSK : WIFI_AUTH_OPEN;
    esp_wifi_disconnect();
    esp_wifi_set_config(WIFI_IF_STA, &w);
    s_down_since = ms();
    esp_wifi_connect();
    rlog("wifi: joining \"%s\"", s_ssid);
}

static void on_event(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    if (base == WIFI_EVENT && id == WIFI_EVENT_STA_DISCONNECTED) {
        wifi_event_sta_disconnected_t *d = data;
        if (s_connected) { rlog("wifi: lost \"%s\" (reason %d)", s_ssid, d->reason); s_down_since = ms(); }
        s_connected = false;
        s_ip[0] = 0;
        s_last_reason = d->reason;
    } else if (base == WIFI_EVENT && id == WIFI_EVENT_AP_STACONNECTED) {
        s_ap_clients++;
    } else if (base == WIFI_EVENT && id == WIFI_EVENT_AP_STADISCONNECTED) {
        if (s_ap_clients > 0) s_ap_clients--;
    } else if (base == IP_EVENT && id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *e = data;
        snprintf(s_ip, sizeof s_ip, IPSTR, IP2STR(&e->ip_info.ip));
        s_connected = true;
        s_last_reason = 0;
        rlog("wifi: on \"%s\" as %s -> http://%s.local or http://%s", s_ssid, s_ip, RB_HOSTNAME, s_ip);
    }
}

/* Reconnect with back-off; hotspot on when the home network is missing or down for 30 s, off again
   once it works and no phone is on the hotspot. */
static void supervisor_task(void *arg)
{
    int64_t next_try = 0;
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(1000));
        if (!s_has_cfg) { ap_enable(true); continue; }
        if (s_connected) {
            if (s_ap_on && s_ap_clients == 0) ap_enable(false);
            continue;
        }
        if (ms() - s_down_since > FALLBACK_AFTER_MS) ap_enable(true);
        if (ms() > next_try) { esp_wifi_connect(); next_try = ms() + 10000; }
    }
}

static void load_cfg(void)
{
    nvs_handle_t h;
    s_has_cfg = false;
    if (nvs_open("wifi", NVS_READONLY, &h) != ESP_OK) return;
    size_t n1 = sizeof s_ssid, n2 = sizeof s_pass;
    if (nvs_get_str(h, "ssid", s_ssid, &n1) == ESP_OK && s_ssid[0]) {
        if (nvs_get_str(h, "pass", s_pass, &n2) != ESP_OK) s_pass[0] = 0;
        s_has_cfg = true;
    }
    nvs_close(h);
}

void wifi_set_sta(const char *ssid, const char *pass)
{
    nvs_handle_t h;
    if (nvs_open("wifi", NVS_READWRITE, &h) == ESP_OK) {
        if (ssid && ssid[0]) { nvs_set_str(h, "ssid", ssid); nvs_set_str(h, "pass", pass ? pass : ""); }
        else { nvs_erase_key(h, "ssid"); nvs_erase_key(h, "pass"); }
        nvs_commit(h);
        nvs_close(h);
    }
    load_cfg();
    if (!s_has_cfg) { s_ssid[0] = s_pass[0] = 0; esp_wifi_disconnect(); s_connected = false; ap_enable(true); rlog("wifi: home network forgotten"); return; }
    sta_connect();
}

void wifi_start(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    s_sta = esp_netif_create_default_wifi_sta();
    s_ap = esp_netif_create_default_wifi_ap();
    esp_netif_set_hostname(s_sta, RB_HOSTNAME);
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    wifi_country_t cc = { .cc = "US", .schan = 1, .nchan = 11, .policy = WIFI_COUNTRY_POLICY_MANUAL };
    esp_wifi_set_country(&cc);
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, on_event, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, on_event, NULL);
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_SOFTAP);
    snprintf(s_ap_ssid, sizeof s_ap_ssid, "KymRadarBase-%02X%02X", mac[4], mac[5]);
    wifi_config_t ap = { 0 };
    strlcpy((char *)ap.ap.ssid, s_ap_ssid, sizeof ap.ap.ssid);
    ap.ap.ssid_len = strlen(s_ap_ssid);
    strlcpy((char *)ap.ap.password, CONFIG_RB_AP_PASSWORD, sizeof ap.ap.password);
    ap.ap.channel = 6;
    ap.ap.max_connection = 2;
    ap.ap.authmode = WIFI_AUTH_WPA2_PSK;
    load_cfg();
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));   /* the AP config can only be set with the AP enabled */
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap));
    ESP_ERROR_CHECK(esp_wifi_start());
    s_ap_on = true;
    if (s_has_cfg) { esp_wifi_set_mode(WIFI_MODE_STA); s_ap_on = false; }
    if (mdns_init() == ESP_OK) {
        mdns_hostname_set(RB_HOSTNAME);
        mdns_instance_name_set("Kymopsis Radar Base");
        mdns_service_add(NULL, "_http", "_tcp", 80, NULL, 0);
    }
    if (s_has_cfg) sta_connect();
    else rlog("wifi: no home network saved; hotspot %s (password %s) -> http://192.168.4.1", s_ap_ssid, CONFIG_RB_AP_PASSWORD);
    xTaskCreate(dns_task, "dns", 3072, NULL, 3, NULL);
    xTaskCreate(supervisor_task, "wifi_sup", 3072, NULL, 3, NULL);
}

bool wifi_ap_active(void) { return s_ap_on; }

static void json_str(char *o, size_t cap, const char *s)   /* minimal JSON string escaping */
{
    size_t k = 0;
    o[k++] = '"';
    for (; *s && k < cap - 3; s++) {
        if (*s == '"' || *s == '\\') o[k++] = '\\';
        o[k++] = (unsigned char)*s < 0x20 ? '?' : *s;
    }
    o[k++] = '"';
    o[k] = 0;
}

void wifi_status_json(char *out, size_t cap)
{
    char ss[80], as[80];
    json_str(ss, sizeof ss, s_ssid);
    json_str(as, sizeof as, s_ap_ssid);
    snprintf(out, cap, "{\"host\":\"%s.local\",\"ap\":{\"on\":%s,\"ssid\":%s,\"ip\":\"192.168.4.1\",\"clients\":%d},"
             "\"sta\":{\"saved\":%s,\"ssid\":%s,\"connected\":%s,\"ip\":\"%s\",\"reason\":%d}}",
             RB_HOSTNAME, s_ap_on ? "true" : "false", as, s_ap_clients, s_has_cfg ? "true" : "false", ss,
             s_connected ? "true" : "false", s_ip, s_last_reason);
}

void wifi_scan_json(char *out, size_t cap)
{
    wifi_scan_config_t sc = { 0 };
    uint16_t n = 20;
    wifi_ap_record_t *r = calloc(n, sizeof *r);
    size_t k = snprintf(out, cap, "[");
    if (r && esp_wifi_scan_start(&sc, true) == ESP_OK && esp_wifi_scan_get_ap_records(&n, r) == ESP_OK) {
        for (int i = 0; i < n && k < cap - 120; i++) {
            char s[80];
            if (!r[i].ssid[0]) continue;
            json_str(s, sizeof s, (const char *)r[i].ssid);
            k += snprintf(out + k, cap - k, "%s{\"ssid\":%s,\"rssi\":%d,\"open\":%s}", k > 1 ? "," : "", s, r[i].rssi,
                          r[i].authmode == WIFI_AUTH_OPEN ? "true" : "false");
        }
    }
    snprintf(out + k, cap - k, "]");
    free(r);
}

void wifi_command(const char *a, const char *b)
{
    if (!a[0]) {
        char s[400];
        wifi_status_json(s, sizeof s);
        rlog("wifi: %s", s);
    } else if (!strcmp(a, "scan")) {
        char *s = malloc(3072);
        if (s) { wifi_scan_json(s, 3072); rlog("wifi scan: %s", s); free(s); }
    } else if (!strcmp(a, "forget")) {
        wifi_set_sta("", "");
    } else {
        wifi_set_sta(a, b);   /* console: no spaces in ssid/password; use the web app for those */
    }
}
