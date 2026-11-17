---
title: Home Assistant
nav: Home Assistant
group: Smart home
---
# Home Assistant

> Coming next. This page describes how the integration will work; it is being built now.

Radar Base will appear in Home Assistant by itself through MQTT discovery. You use your own MQTT broker; the
usual choice is the **Mosquitto broker** add-on in Home Assistant. Kymopsis runs no broker and no cloud.

## Planned entities

| Entity | Type | From |
|---|---|---|
| Occupancy | Binary sensor | 3D presence |
| People count | Sensor | 3D presence |
| Area 1-4 occupied | Binary sensors | 3D presence |
| Fall detected | Binary sensor and event | Fall detection |
| Breathing rate, heart rate | Sensors (diagnostic) | Breathing & heart rate |
| Mounting (ceiling / wall) | Select | 3D presence |

## Planned setup

1. In Home Assistant, install the Mosquitto broker add-on and the MQTT integration.
2. In the Radar Base app: Device settings → Smart home → enter the broker address, user and password.
3. Radar Base shows up under Settings → Devices.

Breathing and heart rate are for wellness only, not for medical use.

Volume, pre-flashed modules or custom firmware: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
