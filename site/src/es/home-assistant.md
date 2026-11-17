---
title: Home Assistant
nav: Home Assistant
group: Smart home
---
# Home Assistant

Radar Base aparece solo en Home Assistant mediante el descubrimiento MQTT. Usas tu propio broker MQTT, normalmente
el complemento **Mosquitto broker** de Home Assistant. Kymopsis no opera ningún broker ni ninguna nube; nada sale de tu red.

## Qué necesitas

- Home Assistant con el complemento **Mosquitto broker** y la integración **MQTT**, o cualquier otro broker MQTT que gestiones tú.
  Anota la dirección del broker, el puerto (normalmente 1883), el usuario y la contraseña.
- Una red Wi-Fi de **2.4 GHz**, o una red de doble banda donde 2.4 GHz y 5 GHz comparten el mismo nombre. El ESP32-S3 no tiene
  radio de 5 GHz.
- Firmware de Radar Base 0.9.1 o posterior ([instálalo desde el navegador](app)).

## Configuración

1. Conéctate a tu Wi-Fi: abre la app de Radar Base (por USB o por el punto de acceso de la placa), Ajustes del dispositivo, Wi-Fi.
2. En Ajustes del dispositivo, Hogar inteligente: introduce la dirección del broker, el puerto, el usuario y la contraseña, y activa MQTT.
3. En Home Assistant abre Ajustes → Dispositivos y servicios → MQTT. **Kymopsis Radar Base** aparece con sus entidades.
   No hay nada más que configurar.

## Entidades

| Entidad | Tipo | Unidad / valores | De la app de radar |
|---|---|---|---|
| Ocupación | Sensor binario | activado / desactivado | Presencia 3D |
| Número de personas | Sensor | número | Presencia 3D |
| Persona más cercana | Sensor | m | Presencia 3D |
| Zona 1, Zona 2, Zona 3, Zona 4 | Sensor binario | activado / desactivado | Presencia 3D |
| Caída detectada | Sensor binario | activado / desactivado | Detección de caídas |
| Frecuencia respiratoria | Sensor | /min | Respiración y frecuencia cardíaca |
| Frecuencia cardíaca | Sensor | /min | Respiración y frecuencia cardíaca |
| App de radar | Sensor | nombre de la app en ejecución | cualquiera |
| Montaje | Selector | techo / pared | Presencia 3D |
| Reiniciar radar | Botón | | cualquiera |

Los valores que la app de radar en ejecución no informa quedan vacíos o desconocidos. La frecuencia respiratoria y cardíaca son solo para bienestar, no para uso médico.

## En línea y sin conexión

La placa publica un topic de disponibilidad. Cuando desactivas MQTT en la placa, el dispositivo pasa a no disponible
al instante. Si la placa pierde la alimentación o el Wi-Fi, el broker la marca como no disponible tras unos 45 segundos; vuelve sola cuando la placa se reconecta.

## Solución de problemas

| Problema | Qué hacer |
|---|---|
| El dispositivo no aparece | Revisa la página Hogar inteligente de la placa: debe decir conectado. Revisa el usuario y la contraseña en el broker. |
| La placa no se conecta al Wi-Fi | Usa una red de 2.4 GHz o una de doble banda con un nombre común; las redes solo de 5 GHz no funcionan. |
| `kymradar.local` no se abre | Una VPN o un proxy en ese ordenador puede quedarse con los nombres `.local`; desactívalo, deja que `.local` vaya directo o abre la dirección IP de la placa. |
| Las distancias salen en pies | Home Assistant usa tu sistema de unidades: Ajustes → Sistema → General → Sistema de unidades: Métrico. |

Volumen, módulos ya grabados o firmware a medida: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
