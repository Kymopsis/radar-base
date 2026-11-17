---
title: ESPHome
nav: ESPHome
group: Smart home
---
# ESPHome

Le composant externe `kymopsis_radar` transforme Radar Base en appareil ESPHome. Il lit les résultats de l'appli radar directement sur l'UART du radar, vous obtenez donc les mêmes valeurs que dans l'appli web et via MQTT : présence, nombre de personnes, personne la plus proche, quatre zones, chute, fréquence respiratoire et cardiaque. Combinez-les avec vos propres capteurs et automatisations ESPHome.

> ESPHome remplace le firmware Radar Base sur l'ESP32. L'appli radar doit démarrer depuis la flash du module radar lui-même (les modules des kits de développement sont livrés ainsi). L'exécution d'applis depuis la mémoire du radar, l'appli web et les données brutes restent réservées au firmware Radar Base.

## Exemple

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

L'exemple complet est `esphome/radar-base.yaml` dans le dépôt.

## Options

| Option | Signification |
|---|---|
| `reset_pin`, `boot_high_pin`, `boot_low_pin` | Broches qui démarrent l'appli radar depuis la flash du module au démarrage (Radar Base : GPIO4, GPIO5, GPIO6) |
| `mount` | `ceiling`, `side` ou `keep` (montage pour l'appli de présence) |
| `timeout` | Efface la présence et le nombre de personnes quand le radar ne signale rien pendant cette durée (3 s par défaut) |

Les valeurs que l'appli radar en cours ne signale pas restent vides ; par exemple, la fréquence respiratoire et cardiaque demande l'appli de signes vitaux. La fréquence respiratoire et cardiaque sert uniquement au bien-être et n'est pas un dispositif médical.

Volumes, modules préflashés ou firmware sur mesure : [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
