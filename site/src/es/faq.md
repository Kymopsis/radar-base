---
title: Preguntas frecuentes y solución de problemas
nav: Preguntas frecuentes
group: Help
---
# Preguntas frecuentes y solución de problemas

## La app se conecta pero no muestra resultados

- ¿Tiene alimentación la placa portadora del radar? ¿Se alimentó primero Radar Base? Abre Diagnóstico → **Reiniciar el radar**.
- Diagnóstico → **Preguntar la versión al radar**: si responde, los cables de resultados (RX0/TX0) están bien.

## Falla la carga de un escenario

- Diagnóstico → **Comprobar el enlace de arranque**. Si no hay respuesta, normalmente hay un cable suelto en RX1, TX1, RST, P12 o P19.
- Vuelve a encajar los cables; los cables dupont pierden contacto con facilidad. Mantenlos por debajo de 15 cm.

## La autoprueba de datos sin procesar muestra bloques dañados o ausentes

- Revisa los cables SPI CLK y MOSI (MOSI, no MISO) y pon un cable de masa al lado.
- Baja la velocidad SPI: consola `cfg spi 12.5`.

## Todos los bloques son malos y solo contienen 00 o FF

El cable de datos está en el pin MISO de la placa portadora. Muévelo a **MOSI**.

## El navegador no muestra el puerto USB

- Usa el puerto marcado como **USB** del controlador, no UART.
- Cierra cualquier terminal serie: solo un programa puede usar el puerto.
- Usa Chrome o Edge en un ordenador de escritorio.

## http://kymradar.local no se abre

Algunos móviles Android no resuelven nombres `.local`. Usa la dirección IP que aparece en Ajustes del dispositivo → Wi-Fi doméstico.

## La frecuencia respiratoria o cardíaca sigue vacía

Siéntate o túmbate sin moverte a 0,5-1,5 m frente al radar, con el pecho hacia él, y espera unos 20 segundos. Estas
mediciones son solo para bienestar, no para uso médico.

## Se muestra una persona pero la zona no se ilumina (o al revés)

Las zonas y el seguimiento son salidas del radar distintas. Una persona muy quieta puede salir del seguimiento mientras
la zona sigue ocupada. La app muestra el recuento más alto del último momento para mantener estable el número.

Volumen, módulos ya grabados o firmware a medida: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
