/* Scenario firmware results -> Radar Base JSON schema v0 lines. See app_json.h and pc-gui/SCHEMA.md.
 *
 * Message layouts (from the scenario app documentation):
 *   3D presence: 0x0A04 targets: int32 n, n x {float x, y, z; int32 doppler index, cluster id}
 *                0x0A0A areas: 4 x uint32 (1 = someone in area k); 0x0A11 mount: uint8 0 top / 1 side
 *                command 0x0201 uint32: 0x13 top mount, 0x14 side mount, 0x15 get mount
 *   Fall:        0x0E02 uint8 is_fall; 0x0F09 uint8 is_human
 *   Vital signs: 0x0A14 float breath /min; 0x0A15 float heart /min; 0x0A16 uint32 flag + float range cm
 *   Gesture:     0x1000 uint16 gesture: 0 up, 1 down, 2 left, 3 right, 4 push, 5 pull (sent once per gesture)
 * The point clouds (0x0A08) are not part of schema v0 and are not translated.
 */
#include "app_json.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "esp_timer.h"
#include "logbuf.h"
#include "usb_fwd.h"
#include "radar_link.h"

#define LINE_MAX 1400
#define MAX_TARGETS 16
#define CAP_REPEAT_MS 5000

typedef enum { APP_NONE, APP_HD3D, APP_FALL, APP_VITAL, APP_GESTURE } app_t;
static const char *const APP_NAME[] = { "none", "hd3d", "fall", "vital", "gesture" };

static volatile bool s_usb_on;
static const app_json_sink_t *s_sink;
static const app_obs_t *s_obs;
static volatile app_t s_app;
static volatile int s_mount = -1;           /* -1 unknown, 0 ceiling, 1 side */
static int64_t s_cap_ms;
static int s_human = -1;                    /* fall app: last is_human */
static float s_br = NAN, s_hr = NAN;
static uint32_t s_lines;

static int64_t now_ms(void) { return esp_timer_get_time() / 1000; }

static bool obs_on(void) { return s_obs && s_obs->active(); }
static bool wanted(void) { return s_usb_on || (s_sink && s_sink->active()) || obs_on(); }

static void emit(const char *s, int n)
{
    if (n <= 0 || n >= LINE_MAX) return;
    if (s_usb_on) usb_fwd_text(s, n);
    if (s_sink && s_sink->active()) s_sink->send(s, n);
    s_lines++;
}

static const char *mount_name(void) { return s_mount == 1 ? "side" : "ceiling"; }

static void send_cap(void)
{
    char s[200];
    int n;
    app_t a = s_app;
    if (a == APP_NONE) return;
    if (a == APP_HD3D)
        n = snprintf(s, sizeof s, "{\"v\":0,\"t\":\"cap\",\"app\":\"hd3d\",\"mounts\":[\"ceiling\",\"side\"],"
                     "\"mount\":\"%s\",\"fov_deg\":55}\n", mount_name());
    else if (a == APP_GESTURE)
        n = snprintf(s, sizeof s, "{\"v\":0,\"t\":\"cap\",\"app\":\"gesture\",\"mounts\":[\"side\"],\"mount\":\"side\"}\n");
    else if (a == APP_FALL)
        n = snprintf(s, sizeof s, "{\"v\":0,\"t\":\"cap\",\"app\":\"fall\",\"mounts\":[\"ceiling\"],\"mount\":\"ceiling\"}\n");
    else
        n = snprintf(s, sizeof s, "{\"v\":0,\"t\":\"cap\",\"app\":\"vital\",\"mounts\":[\"side\"],\"mount\":\"side\"}\n");
    emit(s, n);
    s_cap_ms = now_ms();
    if (obs_on() && a != APP_GESTURE) s_obs->cap(APP_NAME[a], a == APP_HD3D ? mount_name() : a == APP_FALL ? "ceiling" : "side");
}

void app_json_announce(void) { send_cap(); }

static app_t app_of(uint16_t type)
{
    switch (type) {
    case 0x0A04: case 0x0A0A: case 0x0A11: return APP_HD3D;
    case 0x0E02: case 0x0F09: return APP_FALL;
    case 0x0A13: case 0x0A14: case 0x0A15: case 0x0A16: return APP_VITAL;
    case 0x1000: return APP_GESTURE;
    default: return APP_NONE;   /* 0x0A08 (point cloud) is shared by two apps */
    }
}

static void set_app(app_t a)
{
    if (a == APP_NONE || a == s_app) return;
    s_app = a;
    s_human = -1; s_br = s_hr = NAN;
    if (a == APP_HD3D) {
        uint32_t q = 0x15;                   /* ask the 3D app for its mounting */
        radar_tf_send(0x0201, (const uint8_t *)&q, 4);
    }
    rlog("json: scenario %s", APP_NAME[a]);
    send_cap();
}

static float rdf(const uint8_t *p) { float f; memcpy(&f, p, 4); return f; }
static int32_t rdi(const uint8_t *p) { int32_t v; memcpy(&v, p, 4); return v; }

/* %.3f, with JSON null for NaN / inf */
static int fnum(char *s, size_t cap, float f)
{
    return isfinite(f) ? snprintf(s, cap, "%.3f", f) : snprintf(s, cap, "null");
}

static void on_targets(const uint8_t *p, uint16_t len)
{
    if (len < 4) return;
    int32_t n = rdi(p);
    int max = (len - 4) / 20;
    if (n < 0) n = 0;
    if (n > max) n = max;
    if (n > MAX_TARGETS) n = MAX_TARGETS;
    if (obs_on()) {
        float near = NAN;
        for (int i = 0; i < n; i++) { const uint8_t *q = p + 4 + 20 * i; float d = sqrtf(rdf(q) * rdf(q) + rdf(q + 4) * rdf(q + 4));
                                      if (isfinite(d) && (!isfinite(near) || d < near)) near = d; }
        s_obs->targets(n, near);
    }
    static char s[LINE_MAX];   /* only the radar UART0 task calls this; keep it off its stack */
    int k = snprintf(s, sizeof s, "{\"v\":0,\"t\":\"targets\",\"ts\":%lld,\"targets\":[", (long long)now_ms());
    for (int i = 0; i < n; i++) {
        const uint8_t *q = p + 4 + 20 * i;
        k += snprintf(s + k, sizeof s - k, "%s{\"id\":%ld,\"x\":", i ? "," : "", (long)rdi(q + 16));
        k += fnum(s + k, sizeof s - k, rdf(q));
        k += snprintf(s + k, sizeof s - k, ",\"y\":");
        k += fnum(s + k, sizeof s - k, rdf(q + 4));
        k += snprintf(s + k, sizeof s - k, ",\"z\":");
        k += fnum(s + k, sizeof s - k, rdf(q + 8));
        k += snprintf(s + k, sizeof s - k, "}");
    }
    k += snprintf(s + k, sizeof s - k, "]}\n");
    emit(s, k);
}

static void on_zones(const uint8_t *p, uint16_t len)
{
    if (len < 16) return;
    if (obs_on()) {
        bool occ[4];
        for (int i = 0; i < 4; i++) { uint32_t v; memcpy(&v, p + 4 * i, 4); occ[i] = v != 0; }
        s_obs->zones(occ);
    }
    static uint8_t last[16];
    static int64_t last_ms;
    if (!memcmp(last, p, 16) && now_ms() - last_ms < 1000) return;   /* on change, else every 1 s (page clears after 2.5 s) */
    memcpy(last, p, 16);
    last_ms = now_ms();
    char s[200];
    int k = snprintf(s, sizeof s, "{\"v\":0,\"t\":\"zones\",\"zones\":[");
    for (int i = 0; i < 4; i++) {
        uint32_t v;
        memcpy(&v, p + 4 * i, 4);
        k += snprintf(s + k, sizeof s - k, "%s{\"id\":%d,\"occupied\":%s}", i ? "," : "", i + 1, v ? "true" : "false");
    }
    k += snprintf(s + k, sizeof s - k, "]}\n");
    emit(s, k);
}

static void on_fall(int is_fall)
{
    const char *st = s_human == 0 ? "none" : is_fall ? "fall" : "normal";
    if (obs_on()) s_obs->fall(st);
    static const char *last;
    static int64_t last_ms;
    if (st == last && now_ms() - last_ms < 1000) return;   /* on change, else once a second */
    last = st;
    last_ms = now_ms();
    char s[120];
    int k = snprintf(s, sizeof s, "{\"v\":0,\"t\":\"fall\",\"state\":\"%s\"", st);
    if (s_human >= 0) k += snprintf(s + k, sizeof s - k, ",\"count\":%d", s_human ? 1 : 0);
    k += snprintf(s + k, sizeof s - k, "}\n");
    emit(s, k);
}

static void on_vital(int has_dist, float dist_cm)
{
    if (obs_on()) s_obs->vital(s_br > 0 ? s_br : NAN, s_hr > 0 ? s_hr : NAN, has_dist ? dist_cm / 100.0f : NAN);
    char s[200];
    int k = snprintf(s, sizeof s, "{\"v\":0,\"t\":\"vital\",\"ts\":%lld", (long long)now_ms());
    if (isfinite(s_hr) && s_hr > 0) k += snprintf(s + k, sizeof s - k, ",\"hr\":%.1f", s_hr);
    if (isfinite(s_br) && s_br > 0) k += snprintf(s + k, sizeof s - k, ",\"br\":%.1f", s_br);
    if (has_dist && isfinite(dist_cm)) k += snprintf(s + k, sizeof s - k, ",\"dist\":%.2f", dist_cm / 100.0f);
    k += snprintf(s + k, sizeof s - k, "}\n");
    emit(s, k);
}

void app_json_on_tf(uint16_t type, const uint8_t *p, uint16_t len)
{
    if (!wanted()) return;
    app_t a = app_of(type);
    if (a == APP_NONE) return;
    set_app(a);
    if (now_ms() - s_cap_ms > CAP_REPEAT_MS) send_cap();
    switch (type) {
    case 0x0A04: on_targets(p, len); break;
    case 0x0A0A: on_zones(p, len); break;
    case 0x0A11:
        if (len >= 1 && s_mount != (p[0] ? 1 : 0)) { s_mount = p[0] ? 1 : 0; send_cap(); }
        break;
    case 0x0F09:
        if (len >= 1 && s_human != (p[0] ? 1 : 0)) { s_human = p[0] ? 1 : 0; on_fall(0); }
        break;
    case 0x0E02: if (len >= 1) on_fall(p[0] == 1); break;
    case 0x0A14: if (len >= 4) s_br = rdf(p); break;
    case 0x0A15: if (len >= 4) s_hr = rdf(p); break;
    case 0x0A16:
        if (len >= 8) { uint32_t fl; memcpy(&fl, p, 4); on_vital(fl != 0, rdf(p + 4)); }
        break;
    case 0x1000:
        if (len >= 2) {
            static const char *const G[] = { "up", "down", "left", "right", "push", "pull" };
            uint16_t g = (uint16_t)(p[0] | p[1] << 8);
            char s[96];
            int k = snprintf(s, sizeof s, "{\"v\":0,\"t\":\"gesture\",\"ts\":%lld,\"gesture\":\"%s\"}\n",
                             (long long)now_ms(), g < 6 ? G[g] : "unknown");
            emit(s, k);
        }
        break;
    }
}

void app_json_on_cmd(uint16_t type, const uint8_t *p, uint16_t len)
{
    if (type != 0x0201 || len < 4) return;
    uint32_t c;
    memcpy(&c, p, 4);
    if (c != 0x13 && c != 0x14) return;
    int m = c == 0x14 ? 1 : 0;
    if (m != s_mount) { s_mount = m; if (s_app == APP_HD3D && wanted()) send_cap(); }
}

void app_json_usb_enable(bool on)
{
    s_usb_on = on;
    rlog("jsonout: %s (schema v0 lines on USB; scenario %s, mount %s, %lu lines so far)", on ? "on" : "off",
         APP_NAME[s_app], s_mount < 0 ? "unknown" : mount_name(), (unsigned long)s_lines);
    if (on) {
        if (s_app == APP_HD3D) { uint32_t q = 0x15; radar_tf_send(0x0201, (const uint8_t *)&q, 4); }
        send_cap();
    }
}

void app_json_set_sink(const app_json_sink_t *sink) { s_sink = sink; }

void app_json_set_observer(const app_obs_t *obs) { s_obs = obs; }
