---
title: Результаты JSON и API
nav: JSON и API
group: Develop
---
# Результаты JSON и API

Все клиенты - приложение, Home Assistant, ваш собственный код - читают одни и те же результаты: один объект JSON
на строку, версия схемы 0.

## Где их читать

| Подключение | Как |
|---|---|
| USB | Отправьте `jsonout on` в последовательную консоль; строки JSON приходят между строками журнала |
| Wi-Fi | WebSocket `ws://kymradar.local/ws` (одна или несколько строк в сообщении) |

## Сообщения

| `t` | Поля | Примечания |
|---|---|---|
| `cap` | `app` (`hd3d`, `fall`, `vital`), `mounts`, `mount` | Отправляется при запуске сценария, при смене монтажа и каждые 5 s |
| `targets` | `ts` (ms), `targets: [{id, x, y, z}]` | Метры. x вправо, y от радара (плоскость пола при установке на потолке), z вверх |
| `zones` | `zones: [{id, occupied}]` | Зоны 1-4; отправляется при изменении и каждую секунду |
| `fall` | `state` (`none`, `normal`, `fall`), `count` | Отправляется при изменении и каждую секунду |
| `vital` | `ts`, `br`, `hr` (в минуту), `dist` (m) | Частоты не передаются, пока никто не измеряется |

В каждом сообщении есть `"v": 0`. Сообщения с другой версией игнорируйте.

```
{"v":0,"t":"cap","app":"hd3d","mounts":["ceiling","side"],"mount":"side","fov_deg":55}
{"v":0,"t":"targets","ts":42097,"targets":[{"id":1,"x":0.268,"y":1.42,"z":0.05}]}
```

## Команды (консоль USB или приложение)

| Команда | Что делает |
|---|---|
| `ver` | Запрашивает версию радарного приложения |
| `ram <file>` | Запускает радарное приложение из памяти радара |
| `reset` | Перезапускает радар из флеш-памяти |
| `jsonout on/off` | Результаты JSON по USB |
| `tf 0201 13000000` / `14000000` | 3D-присутствие: монтаж на потолок / на стену |
| `cfg [key value]` | Настройки сырых данных |
| `wifi [ssid password]`, `wifi forget`, `wifi scan` | Домашний Wi-Fi |
| `api files/cfg/wifi/scan/mqtt/batch` | Информация об устройстве одной строкой JSON (используется приложением по USB) |
| `mqtt <host> [port] [user] [pass]`, `mqtt off` | Home Assistant через ваш собственный брокер MQTT |
| `board [profile]` | Показать или задать профиль платы |
| `u0raw on/off` | Пересылать порт радарного приложения байт в байт (сырой захват) |
| `batch start <firmware>`, `batch stop` | Пакетная прошивка (прошивка dev-kit) |

## HTTP API (Wi-Fi)

`GET /api/files`, `GET/POST /api/cfg`, `GET/POST /api/wifi`, `GET /api/wifi/scan`, `POST /api/run?op=...`,
`POST /api/tf?type=&data=`, `GET /api/log?since=`. Учётные данные передаются только в теле POST-запроса.

Пример клиента на Python: `python app_views.py --serial COM5` в папке `pc-gui`.

Партии, предварительно прошитые модули или индивидуальная прошивка: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
