---
title: Résultats JSON et API
nav: JSON et API
group: Develop
---
# Résultats JSON et API

Tous les clients - l'appli, Home Assistant, votre propre code - lisent les mêmes résultats : un objet JSON
par ligne, schéma version 0.

## Où les lire

| Connexion | Comment |
|---|---|
| USB | Envoyez `jsonout on` sur la console série ; les lignes JSON arrivent entre les lignes de journal |
| Wi-Fi | WebSocket `ws://kymradar.local/ws` (une ou plusieurs lignes par message) |

## Messages

| `t` | Champs | Remarques |
|---|---|---|
| `cap` | `app` (`hd3d`, `fall`, `vital`), `mounts`, `mount` | Envoyé au démarrage d'un scénario, quand le montage change, et toutes les 5 s |
| `targets` | `ts` (ms), `targets: [{id, x, y, z}]` | Mètres. x à droite, y en s'éloignant du radar (plan du sol quand il est au plafond), z vers le haut |
| `zones` | `zones: [{id, occupied}]` | Zones 1-4 ; envoyé à chaque changement et toutes les secondes |
| `fall` | `state` (`none`, `normal`, `fall`), `count` | Envoyé à chaque changement et toutes les secondes |
| `vital` | `ts`, `br`, `hr` (par minute), `dist` (m) | Fréquences omises tant que personne n'est mesuré |

Chaque message contient `"v": 0`. Ignorez les messages d'une autre version.

```
{"v":0,"t":"cap","app":"hd3d","mounts":["ceiling","side"],"mount":"side","fov_deg":55}
{"v":0,"t":"targets","ts":42097,"targets":[{"id":1,"x":0.268,"y":1.42,"z":0.05}]}
```

## Commandes (console USB ou appli)

| Commande | Ce qu'elle fait |
|---|---|
| `ver` | Demande sa version à l'appli radar |
| `ram <file>` | Exécute une appli radar depuis la mémoire du radar |
| `reset` | Redémarre le radar depuis sa flash |
| `jsonout on/off` | Résultats JSON en USB |
| `tf 0201 13000000` / `14000000` | Présence 3D : montage plafond / mur |
| `cfg [key value]` | Réglages des données brutes |
| `wifi [ssid password]`, `wifi forget`, `wifi scan` | Wi-Fi domestique |
| `api files/cfg/wifi/scan/mqtt/batch` | Infos de l'appareil en une ligne JSON (utilisé par l'appli en USB) |
| `mqtt <host> [port] [user] [pass]`, `mqtt off` | Home Assistant via votre propre broker MQTT |
| `board [profile]` | Afficher ou définir le profil de carte |
| `u0raw on/off` | Transmettre le port de l'appli radar octet par octet (capture brute) |
| `batch start <firmware>`, `batch stop` | Flashage par lots (firmware du kit de développement) |

## API HTTP (Wi-Fi)

`GET /api/files`, `GET/POST /api/cfg`, `GET/POST /api/wifi`, `GET /api/wifi/scan`, `POST /api/run?op=...`,
`POST /api/tf?type=&data=`, `GET /api/log?since=`. Les identifiants ne sont jamais envoyés que dans le corps d'un POST.

Exemple de client en Python : `python app_views.py --serial COM5` dans le dossier `pc-gui`.

Volumes, modules préflashés ou firmware sur mesure : [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
