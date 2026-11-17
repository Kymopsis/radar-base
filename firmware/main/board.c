/* Board profiles. See board.h. */
#include "board.h"
#include <stdio.h>
#include <string.h>
#include "nvs.h"
#include "logbuf.h"

static const board_profile_t PROFILES[] = {
    { "kymopsis-devkit-60g", "Kymopsis Radar Base dev kit (60 GHz)" },
    { "kymopsis-diy-60g", "Kymopsis DIY Radar Base (60 GHz, wired per the guide)" },
};
static char s_id[32];
static bool s_loaded;

static void load(void)
{
    if (s_loaded) return;
    s_loaded = true;
    nvs_handle_t h;
    size_t n = sizeof s_id;
    if (nvs_open("board", NVS_READONLY, &h) == ESP_OK) {
        if (nvs_get_str(h, "id", s_id, &n) != ESP_OK) s_id[0] = 0;
        nvs_close(h);
    }
}

const board_profile_t *board_get(void)
{
    load();
    for (size_t i = 0; i < sizeof PROFILES / sizeof PROFILES[0]; i++)
        if (!strcmp(s_id, PROFILES[i].id)) return &PROFILES[i];
    return NULL;
}

const char *board_id(void) { return board_get() ? s_id : ""; }

int board_set(const char *id)
{
    bool clear = !id[0] || !strcmp(id, "unknown");
    bool known = false;
    for (size_t i = 0; i < sizeof PROFILES / sizeof PROFILES[0]; i++) known |= !strcmp(id, PROFILES[i].id);
    if (!clear && !known) return -1;
    nvs_handle_t h;
    if (nvs_open("board", NVS_READWRITE, &h) == ESP_OK) {
        if (clear) nvs_erase_key(h, "id"); else nvs_set_str(h, "id", id);
        nvs_commit(h);
        nvs_close(h);
    }
    strlcpy(s_id, clear ? "" : id, sizeof s_id);
    s_loaded = true;
    const board_profile_t *b = board_get();
    rlog("board: %s", b ? b->name : "Unknown board, angles not calibrated");
    return 0;
}

int board_json(char *out, int cap)
{
    const board_profile_t *b = board_get();
    int k = snprintf(out, cap, "\"board\":\"%s\",\"board_name\":\"%s\",\"boards\":[", b ? b->id : "",
                     b ? b->name : "Unknown board, angles not calibrated");
    for (size_t i = 0; i < sizeof PROFILES / sizeof PROFILES[0] && k < cap - 120; i++)
        k += snprintf(out + k, cap - k, "%s{\"id\":\"%s\",\"name\":\"%s\"}", i ? "," : "", PROFILES[i].id, PROFILES[i].name);
    k += snprintf(out + k, cap - k, "]");
    return k;
}
