---
title: Primeros pasos
nav: Primeros pasos
group: Start
---
# Primeros pasos

Esta guía te lleva de la caja a resultados de radar en vivo en unos 15 minutos.

## Qué necesitas

- Controlador Radar Base (placa ESP32-S3) y el módulo de radar de 60 GHz en su placa portadora
- 10 cables dupont (hembra-hembra) - incluidos en el kit
- Dos cables USB: uno para Radar Base y otro para alimentar la placa portadora del radar
- Un ordenador con Chrome o Edge (Windows, macOS, Linux o ChromeOS)

## 1. Prepara la placa portadora del radar

Quita todos los puentes (jumpers) del conector serie de la placa (J9) y del conector de arranque P19 / GND / P12.
Si no, el chip USB-serie de la propia placa compite con Radar Base por los mismos cables.

## 2. Cablea

Apaga todo primero. Conecta **GND primero**, luego el resto. Las etiquetas son las impresas en la placa portadora del
radar.

[[wiring]]

| Pin de Radar Base | Etiqueta de la placa del radar | Función |
|---|---|---|
| GND | GND | Masa común (conéctala primero) |
| GPIO17 | RX0 | Comandos a la app del radar |
| GPIO18 | TX0 | Resultados de la app del radar |
| GPIO15 | RX1 | Carga de apps en el radar |
| GPIO16 | TX1 | Respuestas durante la carga |
| GPIO4 | RST | Reinicio del radar |
| GPIO5 | P12 | Selección de arranque (también selección de datos) |
| GPIO6 | P19 | Selección de arranque |
| GPIO12 | SPI CLK | Reloj de datos sin procesar (solo para datos sin procesar) |
| GPIO11 | SPI MOSI | Datos sin procesar (no uses MISO) |

El radar se alimenta desde su propia placa portadora (consume hasta unos 600 mA). No lo alimentes nunca desde el pin
3V3 de la placa del controlador.

## 3. Enciende en el orden correcto

1. Conecta el puerto del controlador marcado como **USB** a tu ordenador. (El puerto marcado como UART solo sirve para
   actualizar el firmware del propio controlador.)
2. Después alimenta la placa portadora del radar.

Radar Base configura los pines de arranque del radar en cuanto se inicia, así que el radar debe arrancar en segundo
lugar. Si los alimentaste al revés, abre Diagnóstico y pulsa **Reiniciar el radar**.

## 4. Míralo funcionar

1. Abre [la app](app/) en Chrome o Edge y pulsa **Conectar USB**; elige el puerto llamado «USB JTAG/serial».
2. Elige **Ejecutar un escenario** → **Presencia 3D y seguimiento**.
3. Camina frente al radar. Tu posición aparece como un punto azul.

Siguiente: [Wi-Fi doméstico](wifi) para el uso diario, o [Datos sin procesar](raw-data) para crear tus propios algoritmos.

Volumen, módulos ya grabados o firmware a medida: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
