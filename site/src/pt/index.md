---
title: Kymopsis Radar Base
nav: Visão geral
---
# Radar Base

O Radar Base conecta um módulo de radar mmWave de 60 GHz ao seu computador, ao seu celular e à sua casa inteligente.
O trabalho de radar é feito pelo próprio radar no seu chip; o Radar Base carrega apps de radar nele, transforma os
resultados em um formato JSON simples e os disponibiliza por USB e Wi-Fi.

[[cards]]
- **Execute um app de radar pronto** | Posições de pessoas em 3D, detecção de quedas, respiração e frequência cardíaca. Sem programar. | getting-started
- **Use no navegador por USB** | Conecte, abra este site no Chrome ou Edge e pronto. Sem Wi-Fi. | usb
- **Coloque na sua rede Wi-Fi de casa** | Acesse em http://kymradar.local de qualquer celular ou computador em casa. | wifi
- **Crie sua própria integração** | Leia os resultados em JSON por USB ou WebSocket a partir de qualquer programa. | json-api
[[/cards]]

## Hardware testado

| Hardware | Status |
|---|---|
| Kit de desenvolvimento Kymopsis Radar Base | Testado. Funciona logo de início; perfil de placa incluído |
| Montagem DIY da Kymopsis: ESP32-S3 DevKit e o módulo de radar de 60 GHz na sua placa do radar, ligados como em [Primeiros passos](getting-started) | Testado. Funciona logo de início; perfil de placa incluído |
| Outras placas e módulos de radar | Não testado. Confira você mesmo a atribuição de pinos e o perfil de placa; o app então mostra "Placa desconhecida, ângulos não calibrados" |

## Edições aberta e de kit de desenvolvimento

| Recurso | Firmware aberto | Firmware do kit de desenvolvimento |
|---|---|---|
| Executar apps de radar a partir da memória do radar, visualizações ao vivo | Sim | Sim |
| JSON por USB e Wi-Fi, app web em 11 idiomas | Sim | Sim |
| Wi-Fi doméstico, hotspot de reserva, Home Assistant (MQTT) | Sim | Sim |
| Fluxo de dados brutos, ajustes e autoteste | Sim | Sim |
| Gravar um app na flash do próprio radar, com backup | - | Sim |
| Gravação em lote: PASS / FAIL, contador, registro CSV | - | Sim |
| Perfil de placa definido na fábrica | - | Sim |

## Como tudo se encaixa

[[diagram]]

| Parte | O que faz |
|---|---|
| Módulo de radar | Executa os apps de radar: cada app faz toda a cadeia de sinal no radar e informa resultados |
| Radar Base (ESP32-S3) | Carrega apps de radar, traduz resultados para JSON, transmite dados brutos, serve o app web, entra no seu Wi-Fi |
| App web | Escolha uma tarefa, acompanhe resultados ao vivo, mude configurações. O mesmo app por Wi-Fi e por USB, em 11 idiomas |
| Seu próprio código | Lê os resultados JSON por USB ou WebSocket |
| Casa inteligente | Home Assistant via MQTT (em breve), ESPHome (depois) |

## Formas de conectar

| Modo | Quando usar | Guia |
|---|---|---|
| USB | Primeiros passos e desenvolvimento. Funciona em qualquer lugar, sem rede | [USB no navegador](usb) |
| Wi-Fi de casa | Uso diário a partir de celulares e computadores em casa | [Wi-Fi de casa](wifi) |
| Hotspot | Primeira configuração, ou quando não há rede doméstica disponível | [Hotspot de reserva](hotspot) |
| Home Assistant | Automações: luzes, alertas, painéis | [Home Assistant](home-assistant) |

O Radar Base nunca precisa de uma conta na nuvem. Seus dados de radar ficam nos seus próprios dispositivos.

Volume, módulos pré-gravados ou firmware personalizado: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
