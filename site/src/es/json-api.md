---
title: Resultados JSON y API
nav: JSON y API
group: Develop
---
# Resultados JSON y API

Todos los clientes - la app, Home Assistant, tu propio código - leen los mismos resultados: un objeto JSON
por línea, versión de esquema 0.

## Dónde leerlos

| Conexión | Cómo |
|---|---|
| USB | Envía `jsonout on` por la consola serie; las líneas JSON llegan entre las líneas de registro |
| Wi-Fi | WebSocket `ws://kymradar.local/ws` (una o más líneas por mensaje) |

## Mensajes

| `t` | Campos | Notas |
|---|---|---|
| `cap` | `app` (`hd3d`, `fall`, `vital`), `mounts`, `mount` | Se envía al iniciar un escenario, al cambiar el montaje y cada 5 s |
| `targets` | `ts` (ms), `targets: [{id, x, y, z}]` | Metros. x a la derecha, y alejándose del radar (plano del suelo si está en el techo), z hacia arriba |
| `zones` | `zones: [{id, occupied}]` | Zonas 1-4; se envía al cambiar y cada segundo |
| `fall` | `state` (`none`, `normal`, `fall`), `count` | Se envía al cambiar y cada segundo |
| `vital` | `ts`, `br`, `hr` (por minuto), `dist` (m) | Las frecuencias se omiten mientras no se mide a nadie |

Cada mensaje lleva `"v": 0`. Ignora los mensajes con otra versión.

```
{"v":0,"t":"cap","app":"hd3d","mounts":["ceiling","side"],"mount":"side","fov_deg":55}
{"v":0,"t":"targets","ts":42097,"targets":[{"id":1,"x":0.268,"y":1.42,"z":0.05}]}
```

## Comandos (consola USB o app)

| Comando | Qué hace |
|---|---|
| `ver` | Pregunta la versión a la app del radar |
| `ram <file>` | Ejecuta una app del radar desde la memoria del radar |
| `reset` | Reinicia el radar desde su flash |
| `jsonout on/off` | Resultados JSON por USB |
| `tf 0201 13000000` / `14000000` | Presencia 3D: montaje en techo / pared |
| `cfg [key value]` | Ajustes de datos sin procesar |
| `wifi [ssid password]`, `wifi forget`, `wifi scan` | Wi-Fi doméstico |
| `api files/cfg/wifi/scan/mqtt/batch` | Información del dispositivo en una línea JSON (la usa la app por USB) |
| `mqtt <host> [port] [user] [pass]`, `mqtt off` | Home Assistant a través de tu propio broker MQTT |
| `board [profile]` | Mostrar o fijar el perfil de placa |
| `u0raw on/off` | Reenviar el puerto de la app del radar byte a byte (captura en bruto) |
| `batch start <firmware>`, `batch stop` | Grabación por lotes (firmware del kit de desarrollo) |

## API HTTP (Wi-Fi)

`GET /api/files`, `GET/POST /api/cfg`, `GET/POST /api/wifi`, `GET /api/wifi/scan`, `POST /api/run?op=...`,
`POST /api/tf?type=&data=`, `GET /api/log?since=`. Las credenciales solo se envían en el cuerpo de un POST.

Cliente de ejemplo en Python: `python app_views.py --serial COM5` en la carpeta `pc-gui`.

Volumen, módulos ya grabados o firmware a medida: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
