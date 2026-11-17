---
title: JSON results and API
nav: JSON and API
group: Develop
---
# JSON results and API

Every client - the app, Home Assistant, your own code - reads the same results: one JSON object per
line, schema version 0.

## Where to read them

| Connection | How |
|---|---|
| USB | Send `jsonout on` on the serial console; JSON lines arrive between log lines |
| Wi-Fi | WebSocket `ws://kymradar.local/ws` (one or more lines per message) |

## Messages

| `t` | Fields | Notes |
|---|---|---|
| `cap` | `app` (`hd3d`, `fall`, `vital`), `mounts`, `mount` | Sent when a scenario starts, when the mounting changes, and every 5 s |
| `targets` | `ts` (ms), `targets: [{id, x, y, z}]` | Metres. x right, y away from the radar (floor plane when on the ceiling), z up |
| `zones` | `zones: [{id, occupied}]` | Areas 1-4; sent on change and every second |
| `fall` | `state` (`none`, `normal`, `fall`), `count` | Sent on change and every second |
| `vital` | `ts`, `br`, `hr` (per minute), `dist` (m) | Rates omitted while nobody is measured |

Every message has `"v": 0`. Ignore messages with another version.

```
{"v":0,"t":"cap","app":"hd3d","mounts":["ceiling","side"],"mount":"side","fov_deg":55}
{"v":0,"t":"targets","ts":42097,"targets":[{"id":1,"x":0.268,"y":1.42,"z":0.05}]}
```

## Commands (USB console or app)

| Command | What it does |
|---|---|
| `ver` | Ask the radar app for its version |
| `ram <file>` | Run a radar app from the radar's memory |
| `reset` | Restart the radar from its flash |
| `jsonout on/off` | JSON results on USB |
| `tf 0201 13000000` / `14000000` | 3D presence: ceiling / wall mounting |
| `cfg [key value]` | Raw-data settings |
| `wifi [ssid password]`, `wifi forget`, `wifi scan` | Home Wi-Fi |
| `api files/cfg/wifi/scan/mqtt/batch` | Device info as one JSON line (used by the app over USB) |
| `mqtt <host> [port] [user] [pass]`, `mqtt off` | Home Assistant over your own MQTT broker |
| `board [profile]` | Show or set the board profile |
| `u0raw on/off` | Forward the radar app port byte for byte (raw capture) |
| `batch start <firmware>`, `batch stop` | Batch flashing (dev-kit firmware) |

## HTTP API (Wi-Fi)

`GET /api/files`, `GET/POST /api/cfg`, `GET/POST /api/wifi`, `GET /api/wifi/scan`, `POST /api/run?op=...`,
`POST /api/tf?type=&data=`, `GET /api/log?since=`. Credentials are only ever sent in a POST body.

Example client in Python: `python app_views.py --serial COM5` in the `pc-gui` folder.

Volume, pre-flashed modules or custom firmware: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
