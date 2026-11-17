---
title: ESPHome
nav: ESPHome
group: Smart home
---
# ESPHome

外部コンポーネント `kymopsis_radar` を使うと、Radar Base を ESPHome デバイスにできます。レーダーアプリの結果をレーダーの UART から直接読むので、Web アプリや MQTT と同じ値が得られます：在室、人数、最も近い人、4 つのエリア、転倒、呼吸と心拍。自分の ESPHome センサーやオートメーションと組み合わせられます。

> ESPHome は ESP32 上の Radar Base ファームウェアを置き換えます。レーダーアプリはレーダーモジュール自身のフラッシュから起動する必要があります（開発キットのモジュールはその状態で出荷されます）。レーダーのメモリからのアプリ実行、Web アプリ、生データは Radar Base ファームウェアで使います。

## 例

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

完全な例はリポジトリの `esphome/radar-base.yaml` にあります。

## オプション

| オプション | 意味 |
|---|---|
| `reset_pin`, `boot_high_pin`, `boot_low_pin` | 起動時にモジュールのフラッシュからレーダーアプリを起動させるピン（Radar Base：GPIO4、GPIO5、GPIO6） |
| `mount` | `ceiling`、`side`、`keep`（在室検知アプリの取り付け方法） |
| `timeout` | レーダーがこの時間何も報告しないとき、在室と人数をクリアします（デフォルト 3 秒） |

実行中のレーダーアプリが報告しない値は空のままです。たとえば呼吸と心拍にはバイタルサインアプリが必要です。呼吸と心拍はウェルネス用途のみで、医療機器ではありません。

大量購入、書き込み済みモジュール、カスタムファームウェア： [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
