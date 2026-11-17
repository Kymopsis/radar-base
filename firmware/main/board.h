#pragma once
/*
 * Board profile: which hardware this Radar Base is. Profiles ship only for Kymopsis boards (their wiring and
 * antenna geometry are known and tested). Any other board runs the same firmware but reports
 * "Unknown board, angles not calibrated". Stored in NVS "board"/"id"; set from the web app, the console
 * (`board <id>`) or at dev-kit provisioning.
 */
typedef struct { const char *id, *name; } board_profile_t;

const board_profile_t *board_get(void);          /* NULL = unknown board */
const char *board_id(void);                      /* "" when unknown */
int board_set(const char *id);                   /* 0 ok, -1 unknown id; "" or "unknown" clears it */
/* JSON fragment: "board":"<id>","board_name":"<name>","boards":[{"id":..,"name":..},...] */
int board_json(char *out, int cap);
