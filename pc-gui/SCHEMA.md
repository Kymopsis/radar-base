# Radar Base JSON schema v0 (results and control)

One line of JSON per message. The same messages go over USB CDC, WebSocket (ESP32 web page) and MQTT/ESPHome. Raw ADC frames are binary ("KYA2" header) and go over USB only.

Coordinates are in metres in the radar frame. x is lateral (right is +). y is depth along boresight (ceiling mount: floor plane). z is height (ceiling) or elevation offset (side).

| `t` | Fields | Notes |
|---|---|---|
| `cap` | `app` (`hd3d`/`fall`/`vital`/`adc`), `mounts` (`["ceiling","side"]`), optional `fov_deg`, `max_range` | Sent once after a scenario starts. The GUI picks pages and views from it. |
| `targets` | `ts` (ms), `targets: [{id, x, y, z?, vx?, vy?}]` | Tracks. The ceiling view draws a square top view, the side view a sector. |
| `zones` | `zones: [{id, x0, y0, x1, y1}]` defines zones; `zones: [{id, occupied}]` updates them | |
| `fall` | `state` (`none`/`normal`/`fall`/`lying`), `height?`, `count?` | |
| `vital` | `ts`, `hr?`, `br?` (per minute), `dist?` (m) | |

Every message carries `"v": 0`. A message with another version is ignored. Optional `app` and `mount` fields on any message switch the GUI.

Viewer: `python app_views.py --demo ceiling|side [--app hd3d|fall|vital]`, or `--replay log.jsonl` for a recorded session.

Since ESP32 firmware v0.8 the ESP32 emits these lines itself (`jsonout on` on the USB console; `app_json.c`), so the PC view, the ESP32 web page and smart-home entities share one contract. Live PC view: `python app_views.py --serial COM5 [--send "tf 0201 14000000"] [--log run.jsonl]`.

What the firmware maps today:

| Scenario firmware message | Schema v0 |
|---|---|
| 3D presence targets (x, y, z, cluster id) | `targets`, `id` = cluster id, no velocity |
| 3D presence 4 areas | `zones` with `id` 1-4 and `occupied` only (no geometry); sent on change and every 2 s |
| 3D presence mount report / mount command | `cap.mount` (`ceiling` / `side`) |
| Fall: is_fall, is_human | `fall.state` `none` / `normal` / `fall`, `count` 0 or 1 |
| Vital: breath rate, heart rate, target flag + range | `vital` `br`, `hr`, `dist` (m), sent with each range report |

`cap` is sent when the scenario is first seen, when the mount changes and every 5 s. Point clouds are not part of v0 and stay on `appout`.
