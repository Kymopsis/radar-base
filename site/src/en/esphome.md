---
title: ESPHome
nav: ESPHome
group: Smart home
---
# ESPHome

The `kymopsis_radar` external component turns Radar Base into an ESPHome device. It reads the results of the radar
app straight from the radar's UART, so you get the same values as the web app and MQTT: occupancy, people, nearest
person, four areas, fall, breathing and heart rate. Mix them with your own ESPHome sensors and automations.

> ESPHome replaces the Radar Base firmware on the ESP32. The radar app must start from the radar module's own flash
> (dev-kit modules ship that way). Running apps from the radar's memory, the web app and raw data stay with the
> Radar Base firmware.

## Example

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

The full example is `esphome/radar-base.yaml` in the repository.

## Options

| Option | Meaning |
|---|---|
| `reset_pin`, `boot_high_pin`, `boot_low_pin` | Pins that start the radar app from the module's flash at boot (Radar Base: GPIO4, GPIO5, GPIO6) |
| `mount` | `ceiling`, `side` or `keep` (presence app mounting) |
| `timeout` | Clear occupancy and people when the radar reports nothing for this long (default 3 s) |

Values that the running radar app does not report stay empty; for example breathing and heart rate need the vital
signs app. Breathing and heart rate are for wellness use only, not a medical device.

Volume, pre-flashed modules or custom firmware: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
