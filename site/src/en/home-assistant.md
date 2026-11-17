---
title: Home Assistant
nav: Home Assistant
group: Smart home
---
# Home Assistant

Radar Base appears in Home Assistant by itself through MQTT discovery. You use your own MQTT broker, usually the
**Mosquitto broker** add-on in Home Assistant. Kymopsis runs no broker and no cloud; nothing leaves your network.

## What you need

- Home Assistant with the **Mosquitto broker** add-on and the **MQTT** integration, or any other MQTT broker you run.
  Note the broker's address, port (usually 1883), user and password.
- A **2.4 GHz** Wi-Fi network, or a dual-band network where 2.4 GHz and 5 GHz share one name. The ESP32-S3 has no
  5 GHz radio.
- Radar Base firmware 0.9.1 or later ([install it from the browser](app)).

## Set it up

1. Join your Wi-Fi: open the Radar Base app (over USB, or the board's hotspot), Device settings, Wi-Fi.
2. In Device settings, Smart home: enter the broker address, port, user and password, then switch MQTT on.
3. In Home Assistant open Settings, Devices & services, MQTT. **Kymopsis Radar Base** is there with its entities.
   Nothing else to configure.

## Entities

| Entity | Type | Unit / values | From radar app |
|---|---|---|---|
| Occupancy | Binary sensor | on / off | 3D presence |
| People | Sensor | count | 3D presence |
| Nearest person | Sensor | m | 3D presence |
| Area 1, Area 2, Area 3, Area 4 | Binary sensors | on / off | 3D presence |
| Fall detected | Binary sensor | on / off | Fall detection |
| Breathing rate | Sensor | /min | Breathing & heart rate |
| Heart rate | Sensor | /min | Breathing & heart rate |
| Radar app | Sensor | name of the running app | any |
| Mounting | Select | ceiling / side | 3D presence |
| Restart radar | Button | | any |

Values that the running radar app does not report stay empty or unknown. Breathing and heart rate are for wellness
only, not for medical use.

## Online and offline

The board publishes an availability topic. When you switch MQTT off on the board, the device turns unavailable at
once. If the board loses power or Wi-Fi, the broker marks it unavailable after about 45 seconds; it comes back by
itself when the board reconnects.

## Troubleshooting

| Problem | What to do |
|---|---|
| The device does not appear | Check the board's Smart home page: it must say connected. Check user and password in the broker. |
| The board does not join Wi-Fi | Use a 2.4 GHz network or a dual-band network with one shared name; 5 GHz-only networks do not work. |
| `kymradar.local` does not open | A VPN or proxy on that computer can take over `.local` names; turn it off or let `.local` go direct, or open the board's IP address. |
| Distances show in feet | Home Assistant uses your unit system: Settings, System, General, Unit system: Metric. |

Volume, pre-flashed modules or custom firmware: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
