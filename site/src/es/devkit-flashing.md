---
title: Módulo de grabación del radar (kit de desarrollo)
nav: Grabación del kit de desarrollo
group: Develop
---
# Módulo de grabación del radar (kit de desarrollo)

El firmware de Radar Base viene en dos ediciones creadas a partir del mismo código fuente:

| Edición | Qué puede hacer |
|---|---|
| Abierta | Todo lo de este sitio: ejecuta cualquier app del radar desde la memoria del radar, JSON, app web, Wi-Fi, hogar inteligente |
| Kit de desarrollo | Abierta + un módulo extra que escribe en la flash del radar |

## Qué hace el módulo

Al escribir una app en la flash del radar, esta arranca sola al encender, sin que Radar Base tenga que cargarla antes. El
módulo hace una copia de seguridad de la flash actual del radar antes de cada escritura; si la copia falla, no se
escribe nada. Las copias se pueden restaurar en cualquier momento desde la app.

## Cómo se entrega el firmware del kit de desarrollo

El firmware del kit de desarrollo viene precargado en la placa del kit. Las actualizaciones se comunican a los compradores por correo electrónico; este firmware no es una descarga pública. El firmware abierto se publicará como código fuente en un repositorio público; esta página lo enlazará cuando esté disponible.

## Grabación por lotes

Para muchos módulos seguidos: elige el firmware una vez y conecta los módulos uno tras otro. Cada módulo se detecta, se copia si quieres, se graba, se verifica, se arranca desde su flash y se le pide su versión. La página y el LED de la placa muestran PASS o FAIL, un contador va sumando y cada módulo recibe una línea en un registro CSV que puedes descargar.

## Qué no hace

- No se conecta a internet.
- No recopila, guarda ni envía ningún dato.
- Nada más en el firmware depende de él.

Es cerrado porque el pequeño programa auxiliar que carga en el radar para escribir no es nuestro para publicarlo. En las
unidades del kit de desarrollo, ese programa se guarda cifrado con una clave única para cada unidad.

## Sin él

Cada app del radar también se ejecuta desde la memoria del radar con el firmware abierto (Ejecutar un escenario). Solo
pierdes la comodidad de «arranca sola sin Radar Base». El firmware abierto, la app y el formato JSON son abiertos por diseño; la app y el formato JSON están documentados en este sitio.

Volumen, módulos ya grabados o firmware a medida: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
