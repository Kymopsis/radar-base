---
title: ESPHome
nav: ESPHome
group: Smart home
---
# ESPHome

`kymopsis_radar` 외부 컴포넌트는 Radar Base를 ESPHome 장치로 만들어 줍니다. 레이더 앱의 결과를 레이더의 UART에서 직접 읽으므로 웹 앱 및 MQTT와 같은 값을 얻습니다: 재실, 사람 수, 가장 가까운 사람, 구역 4개, 낙상, 호흡수와 심박수. 내 ESPHome 센서와 자동화에 자유롭게 섞어 쓸 수 있습니다.

> ESPHome은 ESP32의 Radar Base 펌웨어를 대체합니다. 레이더 앱은 레이더 모듈 자체의 플래시에서 시작해야 합니다(개발 키트 모듈은 그렇게 출하됩니다). 레이더 메모리에서 앱 실행, 웹 앱, 원시 데이터는 Radar Base 펌웨어에서 사용합니다.

## 예시

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

전체 예시는 저장소의 `esphome/radar-base.yaml`입니다.

## 옵션

| 옵션 | 설명 |
|---|---|
| `reset_pin`, `boot_high_pin`, `boot_low_pin` | 부팅할 때 모듈의 플래시에서 레이더 앱을 시작시키는 핀(Radar Base: GPIO4, GPIO5, GPIO6) |
| `mount` | `ceiling`, `side` 또는 `keep`(재실 감지 앱의 설치 방식) |
| `timeout` | 레이더가 이 시간 동안 아무것도 보고하지 않으면 재실과 사람 수를 지웁니다(기본값 3초) |

실행 중인 레이더 앱이 보고하지 않는 값은 비어 있습니다. 예를 들어 호흡수와 심박수에는 활력 징후 앱이 필요합니다. 호흡수와 심박수는 웰니스 용도로만 사용하세요. 의료기기가 아닙니다.

대량 구매, 사전 기록된 모듈, 맞춤 펌웨어: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
