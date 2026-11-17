---
title: ESPHome
nav: ESPHome
group: Smart home
---
# ESPHome

`kymopsis_radar` 外部组件可以把 Radar Base 变成 ESPHome 设备。它直接从雷达的 UART 读取雷达应用的结果，所以得到的数值与网页应用和 MQTT 相同：有人/无人、人数、最近的人、四个区域、跌倒、呼吸和心率。可以把它们与你自己的 ESPHome 传感器和自动化搭配使用。

> ESPHome 会替换 ESP32 上的 Radar Base 固件。雷达应用必须从雷达模块自身的闪存启动（开发套件模块出厂即是如此）。从雷达内存运行应用、网页应用和原始数据仍需使用 Radar Base 固件。

## 示例

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

完整示例见仓库中的 `esphome/radar-base.yaml`。

## 选项

| 选项 | 含义 |
|---|---|
| `reset_pin`, `boot_high_pin`, `boot_low_pin` | 开机时让雷达应用从模块闪存启动的引脚（Radar Base：GPIO4、GPIO5、GPIO6） |
| `mount` | `ceiling`、`side` 或 `keep`（人体存在应用的安装方式） |
| `timeout` | 雷达持续这么久没有任何上报时，清除有人状态和人数（默认 3 秒） |

运行中的雷达应用没有上报的数值会保持为空；例如呼吸和心率需要生命体征应用。呼吸和心率仅用于健康养生用途，不是医疗器械。

批量采购、预刷固件模块或定制固件： [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
