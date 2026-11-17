/* MQTT + Home Assistant discovery. See mqtt_ha.h. */
#include "mqtt_ha.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_mac.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "nvs.h"
#include "mqtt_client.h"
#include "logbuf.h"
#include "app_json.h"
#include "radar_link.h"
#include "usb_fwd.h"

static char s_host[64], s_user[64], s_pass[64], s_id[8], s_base[32];
static int s_port = 1883;
static bool s_on;
static esp_mqtt_client_handle_t s_cli;
static volatile bool s_conn, s_have_ip;

/* last known state (published as one JSON document) */
static struct {
    char app[8], mount[8], fall[8];
    int people;
    float nearest, br, hr, dist;
    bool area[4];
} S = { "none", "ceiling", "none", 0, NAN, NAN, NAN, NAN, { 0 } };
static int64_t s_pub_ms;
static bool s_dirty;
static uint32_t s_pubs;

static int64_t ms(void) { return esp_timer_get_time() / 1000; }

/* ---------------- settings ---------------- */
static void load(void)
{
    nvs_handle_t h;
    s_on = false;
    if (nvs_open("mqtt", NVS_READONLY, &h) != ESP_OK) return;
    size_t n;
    n = sizeof s_host; if (nvs_get_str(h, "host", s_host, &n) != ESP_OK) s_host[0] = 0;
    n = sizeof s_user; if (nvs_get_str(h, "user", s_user, &n) != ESP_OK) s_user[0] = 0;
    n = sizeof s_pass; if (nvs_get_str(h, "pass", s_pass, &n) != ESP_OK) s_pass[0] = 0;
    uint16_t p = 1883; nvs_get_u16(h, "port", &p); s_port = p;
    uint8_t on = 0; nvs_get_u8(h, "on", &on); s_on = on && s_host[0];
    nvs_close(h);
}

/* ---------------- discovery ---------------- */
typedef void (*out_fn)(const char *topic, const char *payload, bool retain);

static void device_json(char *o, size_t cap)
{
    snprintf(o, cap, "\"dev\":{\"ids\":[\"kymradar_%s\"],\"name\":\"Kymopsis Radar Base\",\"mf\":\"Kymopsis\",\"mdl\":\"Radar Base\","
             "\"sw\":\"%s\",\"cu\":\"http://kymradar.local\"},\"avty_t\":\"%s/status\"", s_id, RB_FW_VERSION, s_base);
}

static void one(out_fn out, const char *comp, const char *obj, const char *name, const char *extra)
{
    char t[96], p[720], dev[300];
    device_json(dev, sizeof dev);
    snprintf(t, sizeof t, "homeassistant/%s/kymradar_%s/%s/config", comp, s_id, obj);
    snprintf(p, sizeof p, "{\"name\":\"%s\",\"uniq_id\":\"kymradar_%s_%s\",\"obj_id\":\"kymradar_%s\",\"stat_t\":\"%s/state\",%s,%s}",
             name, s_id, obj, obj, s_base, extra, dev);
    out(t, p, true);
}

static void discovery(out_fn out)
{
    one(out, "binary_sensor", "occupancy", "Occupancy", "\"dev_cla\":\"occupancy\",\"val_tpl\":\"{{ 'ON' if value_json.occupancy else 'OFF' }}\"");
    one(out, "sensor", "people", "People", "\"stat_cla\":\"measurement\",\"ic\":\"mdi:account-group\",\"val_tpl\":\"{{ value_json.people }}\"");
    one(out, "sensor", "nearest", "Nearest person", "\"dev_cla\":\"distance\",\"unit_of_meas\":\"m\",\"stat_cla\":\"measurement\","
        "\"sug_dsp_prc\":1,\"val_tpl\":\"{{ value_json.nearest_m | default(none) }}\"");
    for (int i = 1; i <= 4; i++) {
        char obj[8], name[16], ex[128];
        snprintf(obj, sizeof obj, "area%d", i);
        snprintf(name, sizeof name, "Area %d", i);
        snprintf(ex, sizeof ex, "\"dev_cla\":\"occupancy\",\"val_tpl\":\"{{ 'ON' if value_json.area%d else 'OFF' }}\"", i);
        one(out, "binary_sensor", obj, name, ex);
    }
    one(out, "binary_sensor", "fall", "Fall detected", "\"dev_cla\":\"safety\",\"val_tpl\":\"{{ 'ON' if value_json.fall == 'fall' else 'OFF' }}\"");
    one(out, "sensor", "breathing", "Breathing rate", "\"unit_of_meas\":\"/min\",\"stat_cla\":\"measurement\",\"ent_cat\":\"diagnostic\","
        "\"ic\":\"mdi:lungs\",\"val_tpl\":\"{{ value_json.br | default(none) }}\"");
    one(out, "sensor", "heart_rate", "Heart rate", "\"unit_of_meas\":\"bpm\",\"stat_cla\":\"measurement\",\"ent_cat\":\"diagnostic\","
        "\"ic\":\"mdi:heart-pulse\",\"val_tpl\":\"{{ value_json.hr | default(none) }}\"");
    one(out, "sensor", "scenario", "Radar app", "\"ent_cat\":\"diagnostic\",\"ic\":\"mdi:radar\",\"val_tpl\":\"{{ value_json.scenario }}\"");
    char ex[200];
    snprintf(ex, sizeof ex, "\"cmd_t\":\"%s/mount/set\",\"options\":[\"ceiling\",\"side\"],\"ent_cat\":\"config\",\"val_tpl\":\"{{ value_json.mount }}\"", s_base);
    one(out, "select", "mount", "Mounting", ex);
    snprintf(ex, sizeof ex, "\"cmd_t\":\"%s/restart/set\",\"dev_cla\":\"restart\",\"ent_cat\":\"config\"", s_base);
    one(out, "button", "restart", "Restart radar", ex);
}

static void state_json(char *o, size_t cap)
{
    int k = snprintf(o, cap, "{\"scenario\":\"%s\",\"mount\":\"%s\",\"occupancy\":%s,\"people\":%d",
                     S.app, S.mount, (S.people > 0 || S.area[0] || S.area[1] || S.area[2] || S.area[3]) ? "true" : "false", S.people);
    if (isfinite(S.nearest)) k += snprintf(o + k, cap - k, ",\"nearest_m\":%.2f", S.nearest);
    for (int i = 0; i < 4; i++) k += snprintf(o + k, cap - k, ",\"area%d\":%s", i + 1, S.area[i] ? "true" : "false");
    k += snprintf(o + k, cap - k, ",\"fall\":\"%s\"", S.fall);
    if (isfinite(S.br)) k += snprintf(o + k, cap - k, ",\"br\":%.1f", S.br);
    if (isfinite(S.hr)) k += snprintf(o + k, cap - k, ",\"hr\":%.1f", S.hr);
    if (isfinite(S.dist)) k += snprintf(o + k, cap - k, ",\"dist_m\":%.2f", S.dist);
    snprintf(o + k, cap - k, "}");
}

static void pub(const char *topic, const char *payload, bool retain)
{
    if (s_cli && s_conn) { esp_mqtt_client_enqueue(s_cli, topic, payload, 0, 1, retain, true); s_pubs++; }
}

static void publish_state(void)
{
    char t[48], p[360];
    snprintf(t, sizeof t, "%s/state", s_base);
    state_json(p, sizeof p);
    pub(t, p, true);
    s_dirty = false;
    s_pub_ms = ms();
}

/* ---------------- observer: results from the radar (radar UART task) ---------------- */
static bool obs_active(void) { return true; }   /* cheap; keeps the state current for `mqtt dump` too */
static void mark(void) { s_dirty = true; }
static void o_cap(const char *app, const char *mount)
{
    if (strcmp(S.app, app) || strcmp(S.mount, mount)) {
        strlcpy(S.app, app, sizeof S.app); strlcpy(S.mount, mount, sizeof S.mount);
        S.people = 0; S.nearest = S.br = S.hr = S.dist = NAN; memset(S.area, 0, sizeof S.area); strcpy(S.fall, "none");
        mark();
    }
}
static int s_peak; static int64_t s_peak_ms;
static void o_targets(int n, float nearest)
{
    /* the tracker drops a still person now and then: report the highest count of the last 2 s */
    if (n >= s_peak || ms() - s_peak_ms > 2000) { s_peak = n; s_peak_ms = ms(); }
    if (s_peak != S.people) { S.people = s_peak; mark(); }
    if (n && (!isfinite(S.nearest) || fabsf(nearest - S.nearest) > 0.2f)) { S.nearest = nearest; mark(); }
}
static void o_zones(const bool occ[4]) { if (memcmp(occ, S.area, sizeof S.area)) { memcpy(S.area, occ, sizeof S.area); mark(); } }
static void o_fall(const char *st) { if (strcmp(st, S.fall)) { strlcpy(S.fall, st, sizeof S.fall); mark(); } }
static void o_vital(float br, float hr, float d) { S.br = br; S.hr = hr; S.dist = d; if (ms() - s_pub_ms > 5000) mark(); }
static const app_obs_t s_obs = { obs_active, o_cap, o_targets, o_zones, o_fall, o_vital };

/* ---------------- client ---------------- */
static void on_mqtt(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    esp_mqtt_event_handle_t e = data;
    char t[64];
    if (id == MQTT_EVENT_CONNECTED) {
        s_conn = true;
        snprintf(t, sizeof t, "%s/status", s_base);
        esp_mqtt_client_publish(s_cli, t, "online", 0, 1, 1);
        discovery(pub);
        snprintf(t, sizeof t, "%s/+/set", s_base);
        esp_mqtt_client_subscribe(s_cli, t, 1);
        publish_state();
        rlog("mqtt: connected to %s:%d, Home Assistant discovery sent (%s)", s_host, s_port, s_base);
    } else if (id == MQTT_EVENT_DISCONNECTED) {
        if (s_conn) rlog("mqtt: disconnected");
        s_conn = false;
    } else if (id == MQTT_EVENT_DATA && e->topic_len < 60) {
        char top[64], val[24];
        snprintf(top, sizeof top, "%.*s", e->topic_len, e->topic);
        snprintf(val, sizeof val, "%.*s", e->data_len < 23 ? e->data_len : 23, e->data);
        if (strstr(top, "/mount/set")) {
            uint32_t c = !strcmp(val, "side") ? 0x14 : !strcmp(val, "ceiling") ? 0x13 : 0;
            if (c) { radar_tf_send(0x0201, (const uint8_t *)&c, 4); strlcpy(S.mount, val, sizeof S.mount); mark(); rlog("mqtt: mounting -> %s", val); }
        } else if (strstr(top, "/restart/set")) {
            rlog("mqtt: restart radar requested");
            radar_submit(JOB_RESET_FLASH, "", "");
        }
    } else if (id == MQTT_EVENT_ERROR) {
        rlog("mqtt: connection error (check host, port, user, password)");
    }
}

static void stop_client(void)
{
    if (!s_cli) return;
    if (s_conn) {
        /* a clean disconnect does not fire the last will: mark the device unavailable ourselves */
        char t[48];
        snprintf(t, sizeof t, "%s/status", s_base);
        esp_mqtt_client_publish(s_cli, t, "offline", 0, 1, 1);
        vTaskDelay(pdMS_TO_TICKS(300));
    }
    esp_mqtt_client_stop(s_cli);
    esp_mqtt_client_destroy(s_cli);
    s_cli = NULL;
    s_conn = false;
}

static void start_client(void)
{
    stop_client();
    if (!s_on || !s_have_ip) return;
    char uri[100], lwt[48];
    snprintf(uri, sizeof uri, "mqtt://%s:%d", s_host, s_port);
    snprintf(lwt, sizeof lwt, "%s/status", s_base);
    esp_mqtt_client_config_t c = {
        .broker.address.uri = uri,
        .credentials.username = s_user[0] ? s_user : NULL,
        .credentials.authentication.password = s_pass[0] ? s_pass : NULL,
        .credentials.client_id = s_base + 0,
        .session.last_will = { .topic = lwt, .msg = "offline", .qos = 1, .retain = 1 },
        .session.keepalive = 30,
    };
    s_cli = esp_mqtt_client_init(&c);
    if (!s_cli) { rlog("mqtt: client init failed"); return; }
    esp_mqtt_client_register_event(s_cli, ESP_EVENT_ANY_ID, on_mqtt, NULL);
    esp_mqtt_client_start(s_cli);
    rlog("mqtt: connecting to %s:%d", s_host, s_port);
}

static void on_ip(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    s_have_ip = id == IP_EVENT_STA_GOT_IP;
    if (s_have_ip && s_on && !s_cli) start_client();
}

static void pub_task(void *arg)
{
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(250));
        if (s_conn && (s_dirty || ms() - s_pub_ms > 60000)) publish_state();
    }
}

void mqtt_ha_start(void)
{
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    snprintf(s_id, sizeof s_id, "%02x%02x%02x", mac[3], mac[4], mac[5]);
    snprintf(s_base, sizeof s_base, "kymradar/%s", s_id);
    load();
    app_json_set_observer(&s_obs);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, on_ip, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_LOST_IP, on_ip, NULL);
    xTaskCreate(pub_task, "mqtt_pub", 3072, NULL, 3, NULL);
    if (s_on) rlog("mqtt: enabled for %s:%d (connects when the home Wi-Fi is up)", s_host, s_port);
}

void mqtt_ha_set(const char *host, int port, const char *user, const char *pass, int on)
{
    nvs_handle_t h;
    if (nvs_open("mqtt", NVS_READWRITE, &h) == ESP_OK) {
        if (host) nvs_set_str(h, "host", host);
        if (port > 0) nvs_set_u16(h, "port", (uint16_t)port);
        if (user) nvs_set_str(h, "user", user);
        if (pass) nvs_set_str(h, "pass", pass);
        if (on >= 0) nvs_set_u8(h, "on", (uint8_t)on);
        nvs_commit(h);
        nvs_close(h);
    }
    load();
    rlog("mqtt: %s %s:%d%s", s_on ? "enabled," : "disabled,", s_host, s_port, s_user[0] ? " (with login)" : "");
    if (s_on) start_client(); else stop_client();
    if (s_on && !s_have_ip) rlog("mqtt: waiting for the home Wi-Fi");
}

void mqtt_ha_status_json(char *out, size_t cap)
{
    snprintf(out, cap, "{\"on\":%s,\"host\":\"%s\",\"port\":%d,\"user\":\"%s\",\"has_pass\":%s,\"connected\":%s,\"wifi\":%s,"
             "\"base\":\"%s\",\"published\":%lu}", s_on ? "true" : "false", s_host, s_port, s_user, s_pass[0] ? "true" : "false",
             s_conn ? "true" : "false", s_have_ip ? "true" : "false", s_base, (unsigned long)s_pubs);
}

static void dump_line(const char *topic, const char *payload, bool retain)
{
    char *l = malloc(1024);
    if (!l) return;
    int n = snprintf(l, 1024, "@mqtt %s %s\n", topic, payload);
    usb_fwd_text(l, n);
    free(l);
}

void mqtt_ha_command(const char *a, const char *b, const char *c, const char *d)
{
    if (!a[0]) { char s[300]; mqtt_ha_status_json(s, sizeof s); rlog("mqtt: %s", s); }
    else if (!strcmp(a, "off")) mqtt_ha_set(NULL, 0, NULL, NULL, 0);
    else if (!strcmp(a, "on")) mqtt_ha_set(NULL, 0, NULL, NULL, 1);
    else if (!strcmp(a, "dump")) {
        char t[48], p[360];
        discovery(dump_line);
        snprintf(t, sizeof t, "%s/state", s_base);
        state_json(p, sizeof p);
        dump_line(t, p, true);
        rlog("mqtt dump: done");
    } else mqtt_ha_set(a, b[0] ? atoi(b) : 1883, c, d, 1);
}
