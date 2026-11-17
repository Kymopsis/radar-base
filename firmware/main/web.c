/* Wi-Fi SoftAP + tiny HTTP API for the flasher test page. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include "esp_http_server.h"
#include "esp_spiffs.h"
#include "logbuf.h"
#include "radar_link.h"
#include "app_json.h"
#include "radar_cfg.h"
#include "wifi.h"
#include "mqtt_ha.h"
#include "board.h"

extern const char index_html_start[] asm("_binary_index_html_start");
extern const char index_html_end[] asm("_binary_index_html_end");
extern const char app_html_start[] asm("_binary_app_html_start");
extern const char app_html_end[] asm("_binary_app_html_end");
/* Kymopsis logo, same file as the PC GUI and the website theme */
extern const char logo_start[] asm("_binary_kymopsis_logo_artistic_v4_light_svg_start");
extern const char logo_end[] asm("_binary_kymopsis_logo_artistic_v4_light_svg_end");

/* UI language packs (English is built into the page); served at /i18n/<code>.json, fetched on demand */
extern const char i18n_zh_cn_start[] asm("_binary_zh_CN_json_start");
extern const char i18n_zh_cn_end[] asm("_binary_zh_CN_json_end");
extern const char i18n_zh_tw_start[] asm("_binary_zh_TW_json_start");
extern const char i18n_zh_tw_end[] asm("_binary_zh_TW_json_end");
extern const char i18n_ja_start[] asm("_binary_ja_json_start");
extern const char i18n_ja_end[] asm("_binary_ja_json_end");
extern const char i18n_ko_start[] asm("_binary_ko_json_start");
extern const char i18n_ko_end[] asm("_binary_ko_json_end");
extern const char i18n_de_start[] asm("_binary_de_json_start");
extern const char i18n_de_end[] asm("_binary_de_json_end");
extern const char i18n_fr_start[] asm("_binary_fr_json_start");
extern const char i18n_fr_end[] asm("_binary_fr_json_end");
extern const char i18n_es_start[] asm("_binary_es_json_start");
extern const char i18n_es_end[] asm("_binary_es_json_end");
extern const char i18n_pt_start[] asm("_binary_pt_json_start");
extern const char i18n_pt_end[] asm("_binary_pt_json_end");
extern const char i18n_it_start[] asm("_binary_it_json_start");
extern const char i18n_it_end[] asm("_binary_it_json_end");
extern const char i18n_ru_start[] asm("_binary_ru_json_start");
extern const char i18n_ru_end[] asm("_binary_ru_json_end");
static const struct { const char *code, *start, *end; } s_langs[] = {
    { "zh-CN", i18n_zh_cn_start, i18n_zh_cn_end },
    { "zh-TW", i18n_zh_tw_start, i18n_zh_tw_end },
    { "ja", i18n_ja_start, i18n_ja_end },
    { "ko", i18n_ko_start, i18n_ko_end },
    { "de", i18n_de_start, i18n_de_end },
    { "fr", i18n_fr_start, i18n_fr_end },
    { "es", i18n_es_start, i18n_es_end },
    { "pt", i18n_pt_start, i18n_pt_end },
    { "it", i18n_it_start, i18n_it_end },
    { "ru", i18n_ru_start, i18n_ru_end },
};

static httpd_handle_t s_h;

static bool safe_name(const char *n)
{
    if (!*n || strlen(n) > 60) return false;
    for (const char *p = n; *p; p++)
        if (!(isalnum((unsigned char)*p) || *p == '.' || *p == '_' || *p == '-' || *p == '+')) return false;
    return true;
}

static bool qarg(httpd_req_t *r, const char *key, char *out, size_t cap)
{
    char q[256];
    out[0] = 0;
    if (httpd_req_get_url_query_str(r, q, sizeof q) != ESP_OK) return false;
    return httpd_query_key_value(q, key, out, cap) == ESP_OK;
}

/* application/x-www-form-urlencoded value -> text ('+' = space, %XX) */
static void url_decode(const char *in, char *out, size_t cap)
{
    size_t k = 0;
    for (; *in && k < cap - 1; in++) {
        if (*in == '+') out[k++] = ' ';
        else if (*in == '%' && isxdigit((unsigned char)in[1]) && isxdigit((unsigned char)in[2])) {
            char hx[3] = { in[1], in[2], 0 };
            out[k++] = (char)strtoul(hx, NULL, 16);
            in += 2;
        } else out[k++] = *in;
    }
    out[k] = 0;
}

static esp_err_t h_index(httpd_req_t *r)
{
    httpd_resp_set_type(r, "text/html");
    return httpd_resp_send(r, index_html_start, index_html_end - index_html_start - 1);
}

/* Device info + stored files as JSON (also used by the USB console `api files`). out must hold 8192 bytes. */
void web_files_json(char *out)
{
    size_t k = snprintf(out, 8192, "{");
    k += board_json(out + k, 8192 - k);
    k += snprintf(out + k, 8192 - k, ",\"fw\":\"%s\",\"devkit\":%s,\"busy\":%s,\"pins\":\"%s\",\"files\":[", RB_FW_VERSION,
                        RB_DEVKIT ? "true" : "false", radar_busy() ? "true" : "false", radar_pin_summary());
    DIR *d = opendir("/bins");
    struct dirent *e;
    bool first = true;
    while (d && (e = readdir(d)) && k < 7900) {
        char p[300];
        struct stat st;
        snprintf(p, sizeof p, "/bins/%s", e->d_name);
        if (stat(p, &st)) continue;
        k += snprintf(out + k, 8192 - k, "%s{\"name\":\"%s\",\"size\":%ld}", first ? "" : ",", e->d_name, (long)st.st_size);
        first = false;
    }
    if (d) closedir(d);
    size_t total = 0, used = 0;
    esp_spiffs_info("storage", &total, &used);
    snprintf(out + k, 8192 - k, "],\"used\":%u,\"total\":%u}", (unsigned)used, (unsigned)total);
}

static esp_err_t h_files(httpd_req_t *r)
{
    char *out = malloc(8192);
    if (!out) return httpd_resp_send_err(r, 500, "no memory");
    web_files_json(out);
    httpd_resp_set_type(r, "application/json");
    httpd_resp_sendstr(r, out);
    free(out);
    return ESP_OK;
}

static esp_err_t h_upload(httpd_req_t *r)
{
    char name[64], path[80];
    if (!qarg(r, "name", name, sizeof name) || !safe_name(name)) return httpd_resp_send_err(r, 400, "bad name");
    if (r->content_len > 256 * 1024) return httpd_resp_send_err(r, 400, "file too big");
    snprintf(path, sizeof path, "/bins/%s", name);
    FILE *f = fopen(path, "wb");
    if (!f) return httpd_resp_send_err(r, 500, "open failed");
    char buf[1024];
    int left = r->content_len;
    while (left > 0) {
        int n = httpd_req_recv(r, buf, left < (int)sizeof buf ? left : (int)sizeof buf);
        if (n == HTTPD_SOCK_ERR_TIMEOUT) continue;
        if (n <= 0) { fclose(f); unlink(path); return httpd_resp_send_err(r, 500, "recv failed"); }
        fwrite(buf, 1, n, f);
        left -= n;
    }
    fclose(f);
    rlog("stored %s (%d bytes)", name, (int)r->content_len);
    return httpd_resp_sendstr(r, "ok");
}

static esp_err_t h_delete(httpd_req_t *r)
{
    char name[64], path[80];
    if (!qarg(r, "name", name, sizeof name) || !safe_name(name)) return httpd_resp_send_err(r, 400, "bad name");
    snprintf(path, sizeof path, "/bins/%s", name);
    unlink(path);
    rlog("deleted %s", name);
    return httpd_resp_sendstr(r, "ok");
}

static esp_err_t h_run(httpd_req_t *r)
{
    char op[16], a[64], b[64];
    qarg(r, "op", op, sizeof op);
    qarg(r, "a", a, sizeof a);
    qarg(r, "b", b, sizeof b);
    job_op_t j = JOB_NONE;
    if (!strcmp(op, "version")) j = JOB_VERSION;
    else if (!strcmp(op, "reset")) j = JOB_RESET_FLASH;
    else if (!strcmp(op, "rom")) j = JOB_ROM_PROBE;
    else if (!strcmp(op, "ram") && safe_name(a)) j = JOB_RAM_RUN;
    else if (!strcmp(op, "flash") && safe_name(a) && safe_name(b)) j = JOB_FLASH;
    else if (!strcmp(op, "ota")) j = JOB_OTA_PROBE;
    else if (!strcmp(op, "backup") && safe_name(a)) j = JOB_BACKUP;
    else if (!strcmp(op, "stream") && atoi(a) > 0 && atoi(a) <= 60) {   /* raw-data link self-test, a = seconds */
        strlcpy(b, a, sizeof b);
        strlcpy(a, "adcstream.bin", sizeof a);
        j = JOB_SPI_TEST;
    }
    if (j == JOB_NONE) return httpd_resp_send_err(r, 400, "bad op");
    if (!radar_submit(j, a, b)) return httpd_resp_send_err(r, 409, "busy");
    return httpd_resp_sendstr(r, "queued");
}

static esp_err_t h_log(httpd_req_t *r)
{
    char s[16];
    qarg(r, "since", s, sizeof s);
    char *out = malloc(48 * 1024);
    logbuf_json(strtoul(s, NULL, 10), out, 48 * 1024);
    httpd_resp_set_type(r, "application/json");
    httpd_resp_sendstr(r, out);
    free(out);
    return ESP_OK;
}

/* ---------------- results page + WebSocket (schema v0 JSON lines, see pc-gui/SCHEMA.md) ---------------- */

static esp_err_t h_app(httpd_req_t *r)
{
    httpd_resp_set_type(r, "text/html");
    return httpd_resp_send(r, app_html_start, app_html_end - app_html_start - 1);
}

static esp_err_t h_i18n(httpd_req_t *r)
{
    const char *f = r->uri + strlen("/i18n/");
    for (size_t i = 0; i < sizeof s_langs / sizeof s_langs[0]; i++) {
        size_t n = strlen(s_langs[i].code);
        if (!strncmp(f, s_langs[i].code, n) && !strcmp(f + n, ".json")) {
            httpd_resp_set_type(r, "application/json");
            httpd_resp_set_hdr(r, "Cache-Control", "max-age=3600");
            return httpd_resp_send(r, s_langs[i].start, s_langs[i].end - s_langs[i].start - 1);
        }
    }
    return httpd_resp_send_err(r, 404, "no such language");
}

static esp_err_t h_logo(httpd_req_t *r)
{
    httpd_resp_set_type(r, "image/svg+xml");
    httpd_resp_set_hdr(r, "Cache-Control", "max-age=86400");
    return httpd_resp_send(r, logo_start, logo_end - logo_start - 1);
}

/* POST /api/tf?type=0201&data=13000000 : command to the scenario firmware (mount, fall height, outputs) */
static esp_err_t h_tf(httpd_req_t *r)
{
    char t[8], d[80];
    qarg(r, "type", t, sizeof t);
    qarg(r, "data", d, sizeof d);
    uint16_t type = (uint16_t)strtoul(t, NULL, 16);
    if (type != 0x0201 && type != 0x0E04 && type != 0x010E) return httpd_resp_send_err(r, 400, "type not allowed");
    uint8_t pl[32];
    size_t n = 0;
    for (const char *p = d; isxdigit((unsigned char)p[0]) && isxdigit((unsigned char)p[1]) && n < sizeof pl; p += 2) {
        char hx[3] = { p[0], p[1], 0 };
        pl[n++] = (uint8_t)strtoul(hx, NULL, 16);
    }
    radar_tf_send(type, pl, n);
    rlog("web: tf 0x%04X, %u bytes", type, (unsigned)n);
    return httpd_resp_sendstr(r, "ok");
}

/* ---------------- Wi-Fi + radar settings APIs ---------------- */

static esp_err_t send_json(httpd_req_t *r, const char *s)
{
    httpd_resp_set_type(r, "application/json");
    httpd_resp_set_hdr(r, "Cache-Control", "no-store");
    return httpd_resp_sendstr(r, s);
}

static esp_err_t h_wifi_get(httpd_req_t *r)
{
    char s[400];
    wifi_status_json(s, sizeof s);
    return send_json(r, s);
}

/* POST /api/wifi  body: ssid=...&pass=...  (form-encoded; the password is never put in a URL or a log) */
static esp_err_t h_wifi_set(httpd_req_t *r)
{
    char body[300] = { 0 }, ssid[33] = { 0 }, pass[65] = { 0 }, enc[200];
    if (r->content_len <= 0 || r->content_len >= (int)sizeof body) return httpd_resp_send_err(r, 400, "bad body");
    int got = 0;
    while (got < r->content_len) {
        int n = httpd_req_recv(r, body + got, r->content_len - got);
        if (n == HTTPD_SOCK_ERR_TIMEOUT) continue;
        if (n <= 0) return httpd_resp_send_err(r, 400, "recv failed");
        got += n;
    }
    if (httpd_query_key_value(body, "ssid", enc, sizeof enc) != ESP_OK) return httpd_resp_send_err(r, 400, "ssid missing");
    url_decode(enc, ssid, sizeof ssid);
    if (httpd_query_key_value(body, "pass", enc, sizeof enc) == ESP_OK) url_decode(enc, pass, sizeof pass);
    if (!ssid[0]) return httpd_resp_send_err(r, 400, "ssid empty");
    httpd_resp_sendstr(r, "joining");
    wifi_set_sta(ssid, pass);
    memset(pass, 0, sizeof pass);
    memset(body, 0, sizeof body);
    return ESP_OK;
}

static esp_err_t h_wifi_forget(httpd_req_t *r)
{
    httpd_resp_sendstr(r, "ok");
    wifi_set_sta("", "");
    return ESP_OK;
}

static esp_err_t h_wifi_scan(httpd_req_t *r)
{
    char *s = malloc(3072);
    if (!s) return httpd_resp_send_err(r, 500, "no memory");
    wifi_scan_json(s, 3072);
    esp_err_t e = send_json(r, s);
    free(s);
    return e;
}

static esp_err_t h_cfg_get(httpd_req_t *r)
{
    char s[700];
    radar_cfg_json(s, sizeof s);
    return send_json(r, s);
}

/* POST /api/cfg?k=<key>&v=<value>  (same keys as the console `cfg`); "k=reset" restores defaults */
static esp_err_t h_cfg_set(httpd_req_t *r)
{
    char k[16], v[16], s[760];
    qarg(r, "k", k, sizeof k);
    qarg(r, "v", v, sizeof v);
    radar_cfg_t before = *radar_cfg_get();
    if (!strcmp(k, "reset") || !strcmp(k, "preset")) radar_cfg_command(k, v);
    else if (k[0] && v[0]) radar_cfg_command(k, v);
    else return httpd_resp_send_err(r, 400, "k and v needed");
    bool changed = memcmp(&before, radar_cfg_get(), sizeof before) != 0;
    char j[700];
    radar_cfg_json(j, sizeof j);
    snprintf(s, sizeof s, "{\"applied\":%s,\"cfg\":%s}", changed || !strcmp(k, "reset") ? "true" : "false", j);
    return send_json(r, s);
}

static esp_err_t h_mqtt_get(httpd_req_t *r)
{
    char s[400];
    mqtt_ha_status_json(s, sizeof s);
    return send_json(r, s);
}

/* POST /api/mqtt  body: host=&port=&user=&pass=&on=1  (pass only changes when sent non-empty or with keep=0) */
static esp_err_t h_mqtt_set(httpd_req_t *r)
{
    char body[400] = { 0 }, v[200], host[64] = { 0 }, user[64] = { 0 }, pass[64] = { 0 };
    if (r->content_len <= 0 || r->content_len >= (int)sizeof body) return httpd_resp_send_err(r, 400, "bad body");
    int got = 0;
    while (got < r->content_len) {
        int n = httpd_req_recv(r, body + got, r->content_len - got);
        if (n == HTTPD_SOCK_ERR_TIMEOUT) continue;
        if (n <= 0) return httpd_resp_send_err(r, 400, "recv failed");
        got += n;
    }
    int port = 1883, on = 1;
    if (httpd_query_key_value(body, "host", v, sizeof v) == ESP_OK) url_decode(v, host, sizeof host);
    if (httpd_query_key_value(body, "port", v, sizeof v) == ESP_OK) port = atoi(v);
    if (httpd_query_key_value(body, "user", v, sizeof v) == ESP_OK) url_decode(v, user, sizeof user);
    bool new_pass = httpd_query_key_value(body, "pass", v, sizeof v) == ESP_OK && v[0];
    if (new_pass) url_decode(v, pass, sizeof pass);
    if (httpd_query_key_value(body, "on", v, sizeof v) == ESP_OK) on = atoi(v);
    memset(body, 0, sizeof body);
    if (on && !host[0]) return httpd_resp_send_err(r, 400, "host missing");
    mqtt_ha_set(host[0] ? host : NULL, port, user, new_pass ? pass : NULL, on);
    memset(pass, 0, sizeof pass);
    return h_mqtt_get(r);
}

/* Batch flashing (dev-kit): GET /api/batch status; POST /api/batch?op=start&a=<firmware>&backup=1|0, op=stop */
static esp_err_t h_batch_get(httpd_req_t *r)
{
    char s[400];
    snprintf(s, sizeof s, "{\"available\":false}");
    return send_json(r, s);
}

static esp_err_t h_batch_set(httpd_req_t *r)
{
    return httpd_resp_send_err(r, 404, "included with the Kymopsis development kit firmware");
}

/* GET /api/file?name=<file>: download a stored file (e.g. batchlog.csv, backups) */
static esp_err_t h_file(httpd_req_t *r)
{
    char name[64], path[80], buf[1024];
    if (!qarg(r, "name", name, sizeof name) || !safe_name(name)) return httpd_resp_send_err(r, 400, "bad name");
    snprintf(path, sizeof path, "/bins/%s", name);
    FILE *f = fopen(path, "rb");
    if (!f) return httpd_resp_send_err(r, 404, "no such file");
    httpd_resp_set_type(r, strstr(name, ".csv") ? "text/csv" : "application/octet-stream");
    char cd[96];
    snprintf(cd, sizeof cd, "attachment; filename=\"%s\"", name);
    httpd_resp_set_hdr(r, "Content-Disposition", cd);
    size_t n;
    while ((n = fread(buf, 1, sizeof buf, f)) > 0)
        if (httpd_resp_send_chunk(r, buf, n) != ESP_OK) break;
    fclose(f);
    return httpd_resp_send_chunk(r, NULL, 0);
}

/* POST /api/board?id=<profile id | unknown> */
static esp_err_t h_board(httpd_req_t *r)
{
    char id[32] = "";
    qarg(r, "id", id, sizeof id);
    if (board_set(id)) return httpd_resp_send_err(r, 400, "unknown board profile");
    return httpd_resp_sendstr(r, "ok");
}

/* Captive portal: while the hotspot is on, unknown pages (phone OS connectivity checks) go to the app. */
static esp_err_t h_404(httpd_req_t *r, httpd_err_code_t err)
{
    if (wifi_ap_active()) {
        httpd_resp_set_status(r, "302 Found");
        httpd_resp_set_hdr(r, "Location", "http://192.168.4.1/");
        return httpd_resp_send(r, NULL, 0);
    }
    return httpd_resp_send_err(r, HTTPD_404_NOT_FOUND, "not found");
}

#define WS_MAX 4
static int s_ws_fd[WS_MAX];            /* touched only on the httpd task */
static volatile int s_ws_n;
static volatile int s_ws_pending;

static void ws_drop(int i)
{
    s_ws_fd[i] = s_ws_fd[--s_ws_n];
}

static void ws_send_work(void *arg)
{
    char *s = arg;
    httpd_ws_frame_t f = { .final = true, .type = HTTPD_WS_TYPE_TEXT, .payload = (uint8_t *)s, .len = strlen(s) };
    for (int i = 0; i < s_ws_n; ) {
        if (httpd_ws_get_fd_info(s_h, s_ws_fd[i]) != HTTPD_WS_CLIENT_WEBSOCKET ||
            httpd_ws_send_frame_async(s_h, s_ws_fd[i], &f) != ESP_OK) { ws_drop(i); continue; }
        i++;
    }
    free(s);
    __atomic_sub_fetch(&s_ws_pending, 1, __ATOMIC_RELAXED);
}

static bool ws_active(void) { return s_ws_n > 0; }

static void ws_send(const char *line, int len)
{
    if (__atomic_load_n(&s_ws_pending, __ATOMIC_RELAXED) > 16) return;   /* slow client: drop, never block the radar */
    char *s = malloc(len + 1);
    if (!s) return;
    memcpy(s, line, len);
    s[len] = 0;
    __atomic_add_fetch(&s_ws_pending, 1, __ATOMIC_RELAXED);
    if (httpd_queue_work(s_h, ws_send_work, s) != ESP_OK) { free(s); __atomic_sub_fetch(&s_ws_pending, 1, __ATOMIC_RELAXED); }
}

static const app_json_sink_t s_ws_sink = { ws_active, ws_send };

static void ws_announce_work(void *arg) { app_json_announce(); }

static esp_err_t h_ws(httpd_req_t *r)
{
    if (r->method == HTTP_GET) {       /* handshake done: register the client */
        int fd = httpd_req_to_sockfd(r);
        for (int i = 0; i < s_ws_n; i++)
            if (httpd_ws_get_fd_info(s_h, s_ws_fd[i]) != HTTPD_WS_CLIENT_WEBSOCKET) ws_drop(i--);
        if (s_ws_n < WS_MAX) s_ws_fd[s_ws_n++] = fd;
        rlog("web: results page connected (%d client%s)", s_ws_n, s_ws_n == 1 ? "" : "s");
        httpd_queue_work(s_h, ws_announce_work, NULL);   /* send the current cap to the new page */
        return ESP_OK;
    }
    httpd_ws_frame_t f = { 0 };        /* the page sends nothing we need; read and discard */
    if (httpd_ws_recv_frame(r, &f, 0) != ESP_OK) return ESP_FAIL;
    if (f.len) {
        uint8_t *b = malloc(f.len);
        if (!b) return ESP_ERR_NO_MEM;
        f.payload = b;
        esp_err_t e = httpd_ws_recv_frame(r, &f, f.len);
        free(b);
        return e;
    }
    return ESP_OK;
}

void web_start(void)
{
    httpd_config_t c = HTTPD_DEFAULT_CONFIG();
    c.max_uri_handlers = 20;
    c.uri_match_fn = httpd_uri_match_wildcard;   /* for the /i18n/ language packs */
    c.stack_size = 8192;
    httpd_handle_t h;
    ESP_ERROR_CHECK(httpd_start(&h, &c));
    s_h = h;
    httpd_uri_t u[] = {
        { .uri = "/", .method = HTTP_GET, .handler = h_app },
        { .uri = "/tools", .method = HTTP_GET, .handler = h_index },
        { .uri = "/i18n/*", .method = HTTP_GET, .handler = h_i18n },
        { .uri = "/api/files", .method = HTTP_GET, .handler = h_files },
        { .uri = "/api/upload", .method = HTTP_POST, .handler = h_upload },
        { .uri = "/api/delete", .method = HTTP_POST, .handler = h_delete },
        { .uri = "/api/run", .method = HTTP_POST, .handler = h_run },
        { .uri = "/api/log", .method = HTTP_GET, .handler = h_log },
        { .uri = "/app", .method = HTTP_GET, .handler = h_app },
        { .uri = "/logo.svg", .method = HTTP_GET, .handler = h_logo },
        { .uri = "/api/tf", .method = HTTP_POST, .handler = h_tf },
        { .uri = "/api/wifi", .method = HTTP_GET, .handler = h_wifi_get },
        { .uri = "/api/wifi", .method = HTTP_POST, .handler = h_wifi_set },
        { .uri = "/api/wifi/forget", .method = HTTP_POST, .handler = h_wifi_forget },
        { .uri = "/api/wifi/scan", .method = HTTP_GET, .handler = h_wifi_scan },
        { .uri = "/api/cfg", .method = HTTP_GET, .handler = h_cfg_get },
        { .uri = "/api/mqtt", .method = HTTP_GET, .handler = h_mqtt_get },
        { .uri = "/api/batch", .method = HTTP_GET, .handler = h_batch_get },
        { .uri = "/api/batch", .method = HTTP_POST, .handler = h_batch_set },
        { .uri = "/api/file", .method = HTTP_GET, .handler = h_file },
        { .uri = "/api/board", .method = HTTP_POST, .handler = h_board },
        { .uri = "/api/mqtt", .method = HTTP_POST, .handler = h_mqtt_set },
        { .uri = "/api/cfg", .method = HTTP_POST, .handler = h_cfg_set },
        { .uri = "/ws", .method = HTTP_GET, .handler = h_ws, .is_websocket = true },
    };
    for (size_t i = 0; i < sizeof u / sizeof u[0]; i++) httpd_register_uri_handler(h, &u[i]);
    httpd_register_err_handler(h, HTTPD_404_NOT_FOUND, h_404);
    app_json_set_sink(&s_ws_sink);
}
