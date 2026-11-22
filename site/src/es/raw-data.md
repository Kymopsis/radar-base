---
title: Datos sin procesar y tus propios algoritmos
nav: Datos sin procesar
group: Develop
---
# Datos sin procesar y tus propios algoritmos

Radar Base puede transmitir por USB los datos sin procesar del radar (cada chirp, ambos canales de recepción). El firmware abierto incluye este flujo y sus ajustes; Radar Studio para Windows los muestra y procesa, y abre estas vistas con un kit de desarrollo ([Descargas](downloads)).

## Qué ofrece el firmware abierto

| Función | Dónde |
|---|---|
| Ajustes de datos sin procesar: chirp, ganancia, filtros, antenas | App → Datos sin procesar y ajustes del radar, o consola `cfg <key> <value>` |
| Autoprueba del enlace de datos: se revisa cada bloque | App → Datos sin procesar y ajustes del radar → **Iniciar prueba** |
| Resultados en JSON para tu propio código | [Resultados JSON y API](json-api) |

## Dónde ejecutar un algoritmo

Haz el prototipo en el PC. Luego comprueba si el hardware FFT y CFAR del propio radar puede hacer el trabajo; si es así,
debe ejecutarse en el radar y solo salen los resultados. Si no, prueba con el ESP32, y déjalo solo en el PC si ninguno
de los dos sirve.

## Comprueba el enlace de datos

App → Datos sin procesar y ajustes del radar → **Iniciar prueba**. Ejecuta la app de datos sin procesar unos segundos y
revisa cada bloque: los bloques dañados y los ausentes deben ser 0.

Volumen, módulos ya grabados o firmware a medida: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
