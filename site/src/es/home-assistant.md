---
title: Home Assistant
nav: Home Assistant
group: Smart home
---
# Home Assistant

> Próximamente. Esta página describe cómo funcionará la integración; se está desarrollando ahora.

Radar Base aparecerá solo en Home Assistant mediante el descubrimiento MQTT. Usas tu propio broker MQTT; lo habitual es
el complemento **Mosquitto broker** de Home Assistant. Kymopsis no opera ningún broker ni ninguna nube.

## Entidades previstas

| Entidad | Tipo | Origen |
|---|---|---|
| Ocupación | Sensor binario | Presencia 3D |
| Número de personas | Sensor | Presencia 3D |
| Zona 1-4 ocupada | Sensores binarios | Presencia 3D |
| Caída detectada | Sensor binario y evento | Detección de caídas |
| Frecuencia respiratoria, frecuencia cardíaca | Sensores (diagnóstico) | Respiración y frecuencia cardíaca |
| Montaje (techo / pared) | Selector | Presencia 3D |

## Configuración prevista

1. En Home Assistant, instala el complemento Mosquitto broker y la integración MQTT.
2. En la app de Radar Base: Ajustes del dispositivo → Hogar inteligente → introduce la dirección del broker, el usuario y la contraseña.
3. Radar Base aparece en Ajustes → Dispositivos.

La frecuencia respiratoria y cardíaca son solo para bienestar, no para uso médico.

Volumen, módulos ya grabados o firmware a medida: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
