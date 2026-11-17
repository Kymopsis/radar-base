---
title: ESPHome
nav: ESPHome
group: Smart home
---
# ESPHome

El componente externo `kymopsis_radar` convierte Radar Base en un dispositivo ESPHome. Lee los resultados de la app de radar directamente del UART del radar, así que obtienes los mismos valores que en la app web y en MQTT: presencia, personas, persona más cercana, cuatro zonas, caída, frecuencia respiratoria y cardíaca. Combínalos con tus propios sensores y automatizaciones de ESPHome.

> ESPHome sustituye el firmware de Radar Base en el ESP32. La app de radar debe arrancar desde la flash del propio módulo de radar (los módulos de los kits de desarrollo se envían así). Ejecutar apps desde la memoria del radar, la app web y los datos en bruto siguen siendo del firmware de Radar Base.

## Ejemplo

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

El ejemplo completo es `esphome/radar-base.yaml` en el repositorio.

## Opciones

| Opción | Significado |
|---|---|
| `reset_pin`, `boot_high_pin`, `boot_low_pin` | Pines que arrancan la app de radar desde la flash del módulo al iniciar (Radar Base: GPIO4, GPIO5, GPIO6) |
| `mount` | `ceiling`, `side` o `keep` (montaje de la app de presencia) |
| `timeout` | Borra la presencia y las personas cuando el radar no informa de nada durante este tiempo (3 s por defecto) |

Los valores que la app de radar en ejecución no informa quedan vacíos; por ejemplo, la frecuencia respiratoria y cardíaca necesitan la app de signos vitales. La frecuencia respiratoria y cardíaca son solo para uso de bienestar; no es un dispositivo médico.

Volumen, módulos ya grabados o firmware a medida: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
