---
title: ESPHome
nav: ESPHome
group: Smart home
---
# ESPHome

O componente externo `kymopsis_radar` transforma o Radar Base em um dispositivo ESPHome. Ele lê os resultados do app de radar diretamente do UART do radar, então você recebe os mesmos valores do app web e do MQTT: presença, pessoas, pessoa mais próxima, quatro áreas, queda, frequência respiratória e cardíaca. Combine-os com seus próprios sensores e automações do ESPHome.

> O ESPHome substitui o firmware do Radar Base no ESP32. O app de radar precisa iniciar a partir da flash do próprio módulo de radar (os módulos dos kits de desenvolvimento saem assim). Executar apps da memória do radar, o app web e os dados brutos continuam com o firmware do Radar Base.

## Exemplo

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

O exemplo completo é `esphome/radar-base.yaml` no repositório.

## Opções

| Opção | Significado |
|---|---|
| `reset_pin`, `boot_high_pin`, `boot_low_pin` | Pinos que iniciam o app de radar a partir da flash do módulo na inicialização (Radar Base: GPIO4, GPIO5, GPIO6) |
| `mount` | `ceiling`, `side` ou `keep` (montagem do app de presença) |
| `timeout` | Limpa a presença e as pessoas quando o radar não informa nada por este tempo (padrão 3 s) |

Os valores que o app de radar em execução não informa ficam vazios; por exemplo, a frequência respiratória e cardíaca precisam do app de sinais vitais. A frequência respiratória e cardíaca servem apenas para bem-estar, não é um dispositivo médico.

Volume, módulos pré-gravados ou firmware personalizado: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
