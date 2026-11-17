---
title: ESPHome
nav: ESPHome
group: Smart home
---
# ESPHome

`kymopsis_radar` 外部元件可以把 Radar Base 變成 ESPHome 裝置。它直接從雷達的 UART 讀取雷達應用程式的結果，所以取得的數值與網頁應用程式和 MQTT 相同：有人/無人、人數、最近的人、四個區域、跌倒、呼吸和心率。可以把它們與你自己的 ESPHome 感測器和自動化搭配使用。

> ESPHome 會取代 ESP32 上的 Radar Base 韌體。雷達應用程式必須從雷達模組自身的快閃記憶體啟動（開發套件模組出廠即是如此）。從雷達記憶體執行應用程式、網頁應用程式和原始資料仍需使用 Radar Base 韌體。

## 範例

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

完整範例見儲存庫中的 `esphome/radar-base.yaml`。

## 選項

| 選項 | 說明 |
|---|---|
| `reset_pin`, `boot_high_pin`, `boot_low_pin` | 開機時讓雷達應用程式從模組快閃記憶體啟動的腳位（Radar Base：GPIO4、GPIO5、GPIO6） |
| `mount` | `ceiling`、`side` 或 `keep`（人員存在應用程式的安裝方式） |
| `timeout` | 雷達持續這麼久沒有任何回報時，清除有人狀態和人數（預設 3 秒） |

執行中的雷達應用程式沒有回報的數值會保持為空；例如呼吸和心率需要生命徵象應用程式。呼吸和心率僅用於健康保健用途，非醫療器材。

大量採購、預燒錄模組或客製韌體： [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
