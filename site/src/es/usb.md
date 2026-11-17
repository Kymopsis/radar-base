---
title: Úsalo por USB en el navegador
nav: USB en el navegador
group: Connect
---
# Úsalo por USB en el navegador

La app de Radar Base se ejecuta en Chrome o Edge en tu ordenador y habla con la placa a través del cable USB
(Web Serial). No hay nada que instalar y no interviene el Wi-Fi.

## Pasos

1. Conecta el puerto **USB** del controlador al ordenador.
2. Abre **[radar.kymopsis.com/app](app/)** en Chrome o Edge.
3. Pulsa **Conectar USB** en la barra superior y elige «USB JTAG/serial debug unit».
4. El estado se pone en verde. Elige una tarea en la página de inicio.

Tu navegador pide permiso una vez por sitio para usar el puerto; no se comparte nada más de tu ordenador.

## Qué funciona por USB

| Función | Por USB |
|---|---|
| Ejecutar un escenario, vistas en vivo, ajustes del escenario | Sí |
| Ajustes de datos sin procesar y autoprueba del enlace de datos | Sí |
| Escribir firmware en el radar (kit de desarrollo) | Sí |
| Configurar el Wi-Fi doméstico | Sí (nombres de red y contraseñas sin espacios) |
| Subir o borrar archivos de firmware | No - usa el Wi-Fi para esto |

## Solo un programa a la vez

Un puerto USB solo puede estar abierto en un programa. Cierra cualquier terminal serie antes de
conectar el navegador, y al revés.

## Navegadores

Web Serial está disponible en Chrome, Edge y otros navegadores basados en Chromium en ordenadores de escritorio. Safari
y Firefox no lo admiten; con ellos, o en móviles, usa el [Wi-Fi doméstico](wifi).

Volumen, módulos ya grabados o firmware a medida: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
