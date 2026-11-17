---
title: Dados brutos e seus próprios algoritmos
nav: Dados brutos
group: Develop
---
# Dados brutos e seus próprios algoritmos

O Radar Base pode transmitir por USB os dados brutos do radar (cada chirp, os dois canais de recepção). O firmware aberto inclui esse fluxo e seus ajustes; a ferramenta de PC para visualizar e processar os dados brutos ainda não foi publicada.

## O que o firmware aberto oferece

| Recurso | Onde |
|---|---|
| Ajustes de dados brutos: chirp, ganho, filtros, antenas | App → Dados brutos e ajustes do radar, ou console `cfg <key> <value>` |
| Autoteste do link de dados: cada bloco é verificado | App → Dados brutos e ajustes do radar → **Executar teste** |
| Resultados em JSON para o seu próprio código | [Resultados JSON e API](json-api) |

## Onde executar um algoritmo

Faça o protótipo no PC. Depois verifique se o hardware de FFT e CFAR do próprio radar dá conta; se sim, ele deve rodar
no radar e só os resultados saem dele. Caso contrário, verifique o ESP32, e deixe no PC apenas se nenhum dos dois servir.

## Verifique o link de dados

App → Dados brutos e ajustes do radar → **Executar teste**. Ele executa o app de dados brutos por alguns segundos e
verifica cada bloco: os blocos corrompidos e os ausentes devem ser 0.

Volume, módulos pré-gravados ou firmware personalizado: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
