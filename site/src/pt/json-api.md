---
title: Resultados JSON e API
nav: JSON e API
group: Develop
---
# Resultados JSON e API

Todo cliente - o app, o Home Assistant, o seu próprio código - lê os mesmos resultados: um objeto JSON
por linha, versão de esquema 0.

## Onde lê-los

| Conexão | Como |
|---|---|
| USB | Envie `jsonout on` no console serial; as linhas JSON chegam entre as linhas de log |
| Wi-Fi | WebSocket `ws://kymradar.local/ws` (uma ou mais linhas por mensagem) |

## Mensagens

| `t` | Campos | Observações |
|---|---|---|
| `cap` | `app` (`hd3d`, `fall`, `vital`), `mounts`, `mount` | Enviada quando um cenário inicia, quando a montagem muda e a cada 5 s |
| `targets` | `ts` (ms), `targets: [{id, x, y, z}]` | Metros. x para a direita, y afastando-se do radar (plano do chão quando está no teto), z para cima |
| `zones` | `zones: [{id, occupied}]` | Áreas 1-4; enviada a cada mudança e a cada segundo |
| `fall` | `state` (`none`, `normal`, `fall`), `count` | Enviada a cada mudança e a cada segundo |
| `vital` | `ts`, `br`, `hr` (por minuto), `dist` (m) | As frequências são omitidas enquanto ninguém é medido |

Toda mensagem tem `"v": 0`. Ignore mensagens com outra versão.

```
{"v":0,"t":"cap","app":"hd3d","mounts":["ceiling","side"],"mount":"side","fov_deg":55}
{"v":0,"t":"targets","ts":42097,"targets":[{"id":1,"x":0.268,"y":1.42,"z":0.05}]}
```

## Comandos (console USB ou app)

| Comando | O que faz |
|---|---|
| `ver` | Pergunta a versão ao app do radar |
| `ram <file>` | Executa um app do radar a partir da memória do radar |
| `reset` | Reinicia o radar a partir da flash |
| `jsonout on/off` | Resultados JSON por USB |
| `tf 0201 13000000` / `14000000` | Presença 3D: montagem no teto / na parede |
| `cfg [key value]` | Configurações de dados brutos |
| `wifi [ssid password]`, `wifi forget`, `wifi scan` | Wi-Fi de casa |
| `api files/cfg/wifi/scan/mqtt/batch` | Informações do dispositivo em uma linha JSON (usado pelo app por USB) |
| `mqtt <host> [port] [user] [pass]`, `mqtt off` | Home Assistant pelo seu próprio broker MQTT |
| `board [profile]` | Mostrar ou definir o perfil de placa |
| `u0raw on/off` | Encaminhar a porta do app do radar byte a byte (captura bruta) |
| `batch start <firmware>`, `batch stop` | Gravação em lote (firmware do kit de desenvolvimento) |

## API HTTP (Wi-Fi)

`GET /api/files`, `GET/POST /api/cfg`, `GET/POST /api/wifi`, `GET /api/wifi/scan`, `POST /api/run?op=...`,
`POST /api/tf?type=&data=`, `GET /api/log?since=`. As credenciais só são enviadas no corpo de um POST.

Cliente de exemplo em Python: `python app_views.py --serial COM5` na pasta `pc-gui`.

Volume, módulos pré-gravados ou firmware personalizado: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
