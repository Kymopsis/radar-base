---
title: ESPHome
nav: ESPHome
group: Smart home
---
# ESPHome

Die externe Komponente `kymopsis_radar` macht Radar Base zu einem ESPHome-Gerät. Sie liest die Ergebnisse der Radar-App direkt über den UART des Radars, Sie erhalten also dieselben Werte wie in der Web-App und über MQTT: Anwesenheit, Personenzahl, nächste Person, vier Bereiche, Sturz, Atem- und Herzfrequenz. Kombinieren Sie sie mit Ihren eigenen ESPHome-Sensoren und Automationen.

> ESPHome ersetzt die Radar Base-Firmware auf dem ESP32. Die Radar-App muss aus dem eigenen Flash des Radarmoduls starten (Module der Dev-Kits werden so ausgeliefert). Apps aus dem Speicher des Radars, die Web-App und Rohdaten bleiben der Radar Base-Firmware vorbehalten.

## Beispiel

```
external_components:
  - source:
      type: git
      url: https://github.com/Kymopsis/radar-base
      ref: main
      path: esphome/components
    components: [kymopsis_radar]

uart:
  id: radar_uart
  tx_pin: GPIO17
  rx_pin: GPIO18
  baud_rate: 115200

kymopsis_radar:
  uart_id: radar_uart
  reset_pin: GPIO4
  boot_high_pin: GPIO5
  boot_low_pin: GPIO6
  mount: keep

binary_sensor:
  - platform: kymopsis_radar
    occupancy:
      name: Occupancy
    area_1:
      name: Area 1
    fall:
      name: Fall detected

sensor:
  - platform: kymopsis_radar
    people:
      name: People
    nearest_distance:
      name: Nearest person
    breathing_rate:
      name: Breathing rate
    heart_rate:
      name: Heart rate
```

Das vollständige Beispiel ist `esphome/radar-base.yaml` im Repository.

## Optionen

| Option | Bedeutung |
|---|---|
| `reset_pin`, `boot_high_pin`, `boot_low_pin` | Pins, die beim Start die Radar-App aus dem Flash des Moduls starten (Radar Base: GPIO4, GPIO5, GPIO6) |
| `mount` | `ceiling`, `side` oder `keep` (Montageart der Anwesenheits-App) |
| `timeout` | Anwesenheit und Personenzahl löschen, wenn das Radar so lange nichts meldet (Standard 3 s) |

Werte, die die laufende Radar-App nicht meldet, bleiben leer; Atem- und Herzfrequenz brauchen zum Beispiel die Vitalzeichen-App. Atem- und Herzfrequenz dienen nur dem Wellness-Gebrauch und sind kein Medizinprodukt.

Stückzahlen, vorgeflashte Module oder eigene Firmware: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
