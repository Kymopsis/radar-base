---
title: Pon Radar Base en tu Wi-Fi doméstico
nav: Wi-Fi doméstico
group: Connect
---
# Pon Radar Base en tu Wi-Fi doméstico

Cuando Radar Base está en tu red doméstica, cualquier móvil, tableta u ordenador en casa puede abrirlo en
**http://kymradar.local**. Tus dispositivos siguen en tu Wi-Fi de siempre.

## Configuración

Elige una forma:

- **Desde la app (USB o punto de acceso):** Ajustes del dispositivo → Wi-Fi doméstico → elige tu red → introduce la contraseña → Conectar.
- **Desde un terminal serie:** `wifi <network-name> <password>` (sin espacios en ninguno de los dos).

Radar Base necesita una red de 2,4 GHz. La mayoría de los routers domésticos ofrecen 2,4 GHz y 5 GHz con el mismo nombre, y eso está bien.

## Ábrelo

- En ordenadores, iPhone, iPad y Mac: **http://kymradar.local**
- En algunos móviles Android los nombres `.local` no se resuelven. Usa la dirección IP que aparece en Ajustes del
  dispositivo → Wi-Fi doméstico (por ejemplo http://192.168.1.57), o busca el dispositivo «kymradar» en la lista de tu router.

## Qué pasa con el punto de acceso

El punto de acceso de Radar Base se apaga solo cuando tu Wi-Fi doméstico funciona y ningún móvil está conectado a él. Se
vuelve a encender solo si la red doméstica desaparece durante 30 segundos - consulta [Punto de acceso de respaldo](hotspot).

## Cambiar o quitar la red

Ajustes del dispositivo → Wi-Fi doméstico → **Olvidar red**, o en la consola `wifi forget`. El punto de acceso se vuelve a encender.

## Privacidad

Radar Base solo habla con dispositivos de tu propia red. No tiene conexión con la nube y no envía datos a ningún sitio.

Volumen, módulos ya grabados o firmware a medida: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
