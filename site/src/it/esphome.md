---
title: ESPHome
nav: ESPHome
group: Smart home
---
# ESPHome

Il componente esterno `kymopsis_radar` trasforma Radar Base in un dispositivo ESPHome. Legge i risultati dell'app radar direttamente dalla UART del radar, quindi ottieni gli stessi valori dell'app web e di MQTT: presenza, persone, persona più vicina, quattro aree, caduta, frequenza respiratoria e cardiaca. Combinali con i tuoi sensori e le tue automazioni ESPHome.

> ESPHome sostituisce il firmware di Radar Base sull'ESP32. L'app radar deve partire dalla flash del modulo radar stesso (i moduli dei kit di sviluppo vengono spediti così). L'esecuzione di app dalla memoria del radar, l'app web e i dati grezzi restano al firmware di Radar Base.

## Esempio

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

L'esempio completo è `esphome/radar-base.yaml` nel repository.

## Opzioni

| Opzione | Significato |
|---|---|
| `reset_pin`, `boot_high_pin`, `boot_low_pin` | Pin che avviano l'app radar dalla flash del modulo all'avvio (Radar Base: GPIO4, GPIO5, GPIO6) |
| `mount` | `ceiling`, `side` o `keep` (montaggio dell'app di presenza) |
| `timeout` | Azzera presenza e persone quando il radar non segnala nulla per questo tempo (predefinito 3 s) |

I valori che l'app radar in esecuzione non segnala restano vuoti; per esempio, frequenza respiratoria e cardiaca richiedono l'app dei parametri vitali. Frequenza respiratoria e cardiaca sono solo per il benessere, non sono un dispositivo medico.

Volumi, moduli già programmati o firmware personalizzato: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
