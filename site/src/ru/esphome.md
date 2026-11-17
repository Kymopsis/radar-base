---
title: ESPHome
nav: ESPHome
group: Smart home
---
# ESPHome

Внешний компонент `kymopsis_radar` превращает Radar Base в устройство ESPHome. Он читает результаты приложения радара прямо из UART радара, так что вы получаете те же значения, что в веб-приложении и MQTT: присутствие, число людей, ближайший человек, четыре зоны, падение, дыхание и пульс. Сочетайте их с собственными датчиками и автоматизациями ESPHome.

> ESPHome заменяет прошивку Radar Base на ESP32. Приложение радара должно запускаться из собственной флеш-памяти модуля радара (модули комплектов разработчика поставляются именно так). Запуск приложений из памяти радара, веб-приложение и сырые данные остаются за прошивкой Radar Base.

## Пример

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

Полный пример — `esphome/radar-base.yaml` в репозитории.

## Параметры

| Параметр | Значение |
|---|---|
| `reset_pin`, `boot_high_pin`, `boot_low_pin` | Выводы, которые при загрузке запускают приложение радара из флеш-памяти модуля (Radar Base: GPIO4, GPIO5, GPIO6) |
| `mount` | `ceiling`, `side` или `keep` (установка приложения присутствия) |
| `timeout` | Сбросить присутствие и число людей, если радар так долго ничего не сообщает (по умолчанию 3 с) |

Значения, которые работающее приложение радара не сообщает, остаются пустыми; например, дыхание и пульс требуют приложения жизненных показателей. Дыхание и пульс предназначены только для оздоровительных целей и не являются медицинским изделием.

Партии, предварительно прошитые модули или индивидуальная прошивка: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
