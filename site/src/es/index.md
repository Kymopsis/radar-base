---
title: Kymopsis Radar Base
nav: Resumen
---
# Radar Base

Radar Base conecta un módulo de radar mmWave de 60 GHz con tu ordenador, tu móvil y tu hogar inteligente.
El trabajo de radar lo hace el propio radar en su chip; Radar Base le carga apps de radar, convierte sus resultados en
un formato JSON sencillo y los ofrece por USB y Wi-Fi.

[[cards]]
- **Ejecuta una app de radar lista para usar** | Posiciones de personas en 3D, detección de caídas, respiración y frecuencia cardíaca. Sin programar. | getting-started
- **Úsalo en el navegador por USB** | Conéctalo, abre este sitio en Chrome o Edge y listo. No hace falta Wi-Fi. | usb
- **Ponlo en tu Wi-Fi doméstico** | Accede en http://kymradar.local desde cualquier móvil u ordenador en casa. | wifi
- **Crea tu propia integración** | Lee los resultados como JSON por USB o WebSocket desde cualquier programa. | json-api
[[/cards]]

## Hardware probado

| Hardware | Estado |
|---|---|
| Kit de desarrollo Kymopsis Radar Base | Probado. Funciona nada más sacarlo de la caja; perfil de placa incluido |
| Montaje DIY de Kymopsis: ESP32-S3 DevKit y el módulo de radar de 60 GHz en su placa portadora, cableados como en [Primeros pasos](getting-started) | Probado. Funciona nada más sacarlo de la caja; perfil de placa incluido |
| Otras placas y módulos de radar | Sin probar. Comprueba tú la asignación de pines y el perfil de placa; la app mostrará entonces «Placa desconocida, ángulos sin calibrar» |

## Ediciones abierta y de kit de desarrollo

| Función | Firmware abierto | Firmware del kit de desarrollo |
|---|---|---|
| Ejecutar apps de radar desde la memoria del radar, vistas en directo | Sí | Sí |
| JSON por USB y Wi-Fi, app web en 11 idiomas | Sí | Sí |
| Wi-Fi doméstico, punto de acceso de respaldo, Home Assistant (MQTT) | Sí | Sí |
| Flujo de datos sin procesar, ajustes y autoprueba | Sí | Sí |
| Grabar una app en la flash propia del radar, con copia de seguridad | - | Sí |
| Grabación por lotes: PASS / FAIL, contador, registro CSV | - | Sí |
| Perfil de placa fijado en fábrica | - | Sí |

## Cómo encaja todo

[[diagram]]

| Parte | Qué hace |
|---|---|
| Módulo de radar | Ejecuta las apps de radar: cada app hace toda la cadena de señal en el radar y envía resultados |
| Radar Base (ESP32-S3) | Carga apps de radar, traduce los resultados a JSON, transmite datos sin procesar, sirve la app web, se une a tu Wi-Fi |
| App web | Elige una tarea, mira resultados en vivo, cambia ajustes. La misma app por Wi-Fi y por USB, en 11 idiomas |
| Tu propio código | Lee los resultados JSON por USB o WebSocket |
| Hogar inteligente | Home Assistant con MQTT (detección automática), componente ESPHome |

## Formas de conectar

| Modo | Cuándo usarlo | Guía |
|---|---|---|
| USB | Primeros pasos y desarrollo. Funciona en cualquier sitio, sin red | [USB en el navegador](usb) |
| Wi-Fi doméstico | Uso diario desde móviles y ordenadores en casa | [Wi-Fi doméstico](wifi) |
| Punto de acceso | Primera configuración, o cuando no hay red doméstica disponible | [Punto de acceso de respaldo](hotspot) |
| Home Assistant | Automatizaciones: luces, alertas, paneles | [Home Assistant](home-assistant) |

Radar Base nunca necesita una cuenta en la nube. Tus datos de radar se quedan en tus propios dispositivos.

Volumen, módulos ya grabados o firmware a medida: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
