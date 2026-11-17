---
title: Risultati JSON e API
nav: JSON e API
group: Develop
---
# Risultati JSON e API

Ogni client - l'app, Home Assistant, il tuo codice - legge gli stessi risultati: un oggetto JSON per
riga, versione dello schema 0.

## Dove leggerli

| Connessione | Come |
|---|---|
| USB | Invia `jsonout on` sulla console seriale; le righe JSON arrivano tra le righe di log |
| Wi-Fi | WebSocket `ws://kymradar.local/ws` (una o più righe per messaggio) |

## Messaggi

| `t` | Campi | Note |
|---|---|---|
| `cap` | `app` (`hd3d`, `fall`, `vital`), `mounts`, `mount` | Inviato all'avvio di uno scenario, quando cambia il montaggio e ogni 5 s |
| `targets` | `ts` (ms), `targets: [{id, x, y, z}]` | Metri. x a destra, y in allontanamento dal radar (piano del pavimento se è a soffitto), z verso l'alto |
| `zones` | `zones: [{id, occupied}]` | Aree 1-4; inviato a ogni cambiamento e ogni secondo |
| `fall` | `state` (`none`, `normal`, `fall`), `count` | Inviato a ogni cambiamento e ogni secondo |
| `vital` | `ts`, `br`, `hr` (al minuto), `dist` (m) | Le frequenze sono omesse finché nessuno viene misurato |

Ogni messaggio ha `"v": 0`. Ignora i messaggi con un'altra versione.

```
{"v":0,"t":"cap","app":"hd3d","mounts":["ceiling","side"],"mount":"side","fov_deg":55}
{"v":0,"t":"targets","ts":42097,"targets":[{"id":1,"x":0.268,"y":1.42,"z":0.05}]}
```

## Comandi (console USB o app)

| Comando | Cosa fa |
|---|---|
| `ver` | Chiede la versione all'app radar |
| `ram <file>` | Esegue un'app radar dalla memoria del radar |
| `reset` | Riavvia il radar dalla sua flash |
| `jsonout on/off` | Risultati JSON via USB |
| `tf 0201 13000000` / `14000000` | Presenza 3D: montaggio a soffitto / a parete |
| `cfg [key value]` | Impostazioni dei dati grezzi |
| `wifi [ssid password]`, `wifi forget`, `wifi scan` | Wi-Fi di casa |
| `api files/cfg/wifi/scan/mqtt/batch` | Info del dispositivo in una riga JSON (usato dall'app via USB) |
| `mqtt <host> [port] [user] [pass]`, `mqtt off` | Home Assistant tramite il tuo broker MQTT |
| `board [profile]` | Mostra o imposta il profilo scheda |
| `u0raw on/off` | Inoltra la porta dell'app radar byte per byte (acquisizione grezza) |
| `batch start <firmware>`, `batch stop` | Scrittura in serie (firmware del kit di sviluppo) |

## API HTTP (Wi-Fi)

`GET /api/files`, `GET/POST /api/cfg`, `GET/POST /api/wifi`, `GET /api/wifi/scan`, `POST /api/run?op=...`,
`POST /api/tf?type=&data=`, `GET /api/log?since=`. Le credenziali vengono inviate solo nel corpo di una POST.

Client di esempio in Python: `python app_views.py --serial COM5` nella cartella `pc-gui`.

Volumi, moduli già programmati o firmware personalizzato: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
