---
title: Home Assistant
nav: Home Assistant
group: Smart home
---
# Home Assistant

> Em breve. Esta página descreve como a integração vai funcionar; ela está sendo desenvolvida agora.

O Radar Base vai aparecer sozinho no Home Assistant por meio da descoberta MQTT. Você usa o seu próprio broker MQTT; o
mais comum é o complemento **Mosquitto broker** do Home Assistant. A Kymopsis não opera broker nem nuvem.

## Entidades planejadas

| Entidade | Tipo | Origem |
|---|---|---|
| Ocupação | Sensor binário | Presença 3D |
| Número de pessoas | Sensor | Presença 3D |
| Área 1-4 ocupada | Sensores binários | Presença 3D |
| Queda detectada | Sensor binário e evento | Detecção de quedas |
| Frequência respiratória, frequência cardíaca | Sensores (diagnóstico) | Respiração e frequência cardíaca |
| Montagem (teto / parede) | Seleção | Presença 3D |

## Configuração planejada

1. No Home Assistant, instale o complemento Mosquitto broker e a integração MQTT.
2. No app do Radar Base: Configurações do dispositivo → Casa inteligente → informe o endereço do broker, o usuário e a senha.
3. O Radar Base aparece em Configurações → Dispositivos.

A frequência respiratória e a cardíaca são apenas para bem-estar, não para uso médico.

Volume, módulos pré-gravados ou firmware personalizado: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
