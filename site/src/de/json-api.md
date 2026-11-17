---
title: JSON-Ergebnisse und API
nav: JSON und API
group: Develop
---
# JSON-Ergebnisse und API

Jeder Client - die App, Home Assistant, Ihr eigener Code - liest dieselben Ergebnisse: ein JSON-Objekt
pro Zeile, Schemaversion 0.

## Wo man sie liest

| Verbindung | Wie |
|---|---|
| USB | Senden Sie `jsonout on` an die serielle Konsole; JSON-Zeilen kommen zwischen den Log-Zeilen an |
| Wi-Fi | WebSocket `ws://kymradar.local/ws` (eine oder mehrere Zeilen pro Nachricht) |

## Nachrichten

| `t` | Felder | Hinweise |
|---|---|---|
| `cap` | `app` (`hd3d`, `fall`, `vital`), `mounts`, `mount` | Wird gesendet, wenn ein Szenario startet, wenn sich die Montage ändert, und alle 5 s |
| `targets` | `ts` (ms), `targets: [{id, x, y, z}]` | Meter. x rechts, y vom Radar weg (Bodenebene bei Deckenmontage), z nach oben |
| `zones` | `zones: [{id, occupied}]` | Bereiche 1-4; bei Änderung und jede Sekunde gesendet |
| `fall` | `state` (`none`, `normal`, `fall`), `count` | Bei Änderung und jede Sekunde gesendet |
| `vital` | `ts`, `br`, `hr` (pro Minute), `dist` (m) | Raten entfallen, solange niemand gemessen wird |

Jede Nachricht enthält `"v": 0`. Ignorieren Sie Nachrichten mit einer anderen Version.

```
{"v":0,"t":"cap","app":"hd3d","mounts":["ceiling","side"],"mount":"side","fov_deg":55}
{"v":0,"t":"targets","ts":42097,"targets":[{"id":1,"x":0.268,"y":1.42,"z":0.05}]}
```

## Befehle (USB-Konsole oder App)

| Befehl | Was er tut |
|---|---|
| `ver` | Fragt die Radar-App nach ihrer Version |
| `ram <file>` | Startet eine Radar-App aus dem Speicher des Radars |
| `reset` | Startet das Radar aus seinem Flash neu |
| `jsonout on/off` | JSON-Ergebnisse über USB |
| `tf 0201 13000000` / `14000000` | 3D-Anwesenheit: Decken- / Wandmontage |
| `cfg [key value]` | Rohdaten-Einstellungen |
| `wifi [ssid password]`, `wifi forget`, `wifi scan` | Heim-Wi-Fi |
| `api files/cfg/wifi/scan/mqtt/batch` | Geräteinfos als eine JSON-Zeile (von der App über USB genutzt) |
| `mqtt <host> [port] [user] [pass]`, `mqtt off` | Home Assistant über Ihren eigenen MQTT-Broker |
| `board [profile]` | Board-Profil anzeigen oder setzen |
| `u0raw on/off` | Den Port der Radar-App Byte für Byte weiterleiten (Rohmitschnitt) |
| `batch start <firmware>`, `batch stop` | Stapel-Flashen (Dev-Kit-Firmware) |

## HTTP-API (Wi-Fi)

`GET /api/files`, `GET/POST /api/cfg`, `GET/POST /api/wifi`, `GET /api/wifi/scan`, `POST /api/run?op=...`,
`POST /api/tf?type=&data=`, `GET /api/log?since=`. Zugangsdaten werden nur im POST-Body gesendet.

Beispiel-Client in Python: `python app_views.py --serial COM5` im Ordner `pc-gui`.

Stückzahlen, vorgeflashte Module oder eigene Firmware: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
