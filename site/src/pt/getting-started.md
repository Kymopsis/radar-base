---
title: Primeiros passos
nav: Primeiros passos
group: Start
---
# Primeiros passos

Este guia leva você da caixa até resultados de radar ao vivo em cerca de 15 minutos.

## O que você precisa

- Controlador Radar Base (placa ESP32-S3) e o módulo de radar de 60 GHz na sua placa
- 10 jumpers (fêmea-fêmea) - incluídos no kit
- Dois cabos USB: um para o Radar Base e outro para alimentar a placa do radar
- Um computador com Chrome ou Edge (Windows, macOS, Linux ou ChromeOS)

## 1. Prepare a placa do radar

Retire todas as tampas de jumper do conector serial da placa (J9) e do conector de boot P19 / GND / P12.
Caso contrário, o chip USB-serial da própria placa disputa com o Radar Base os mesmos fios.

## 2. Faça a fiação

Desligue tudo primeiro. Conecte **o GND primeiro** e depois o resto. Os nomes são os impressos na placa do radar.

[[wiring]]

| Pino do Radar Base | Nome na placa do radar | Função |
|---|---|---|
| GND | GND | Terra comum (conecte primeiro) |
| GPIO17 | RX0 | Comandos para o app do radar |
| GPIO18 | TX0 | Resultados do app do radar |
| GPIO15 | RX1 | Carregar apps no radar |
| GPIO16 | TX1 | Respostas durante o carregamento |
| GPIO4 | RST | Reset do radar |
| GPIO5 | P12 | Seleção de boot (também seleção de dados) |
| GPIO6 | P19 | Seleção de boot |
| GPIO12 | SPI CLK | Clock dos dados brutos (somente para dados brutos) |
| GPIO11 | SPI MOSI | Dados brutos (não use MISO) |

O radar é alimentado pela própria placa (consome até cerca de 600 mA). Nunca o alimente pelo pino 3V3 da placa do
controlador.

## 3. Ligue na ordem certa

1. Conecte a porta do controlador marcada como **USB** ao seu computador. (A porta marcada como UART serve apenas para
   atualizar o firmware do próprio controlador.)
2. Depois alimente a placa do radar.

O Radar Base configura os pinos de boot do radar assim que inicia, então o radar precisa ligar em segundo lugar. Se você
os ligou na ordem inversa, abra Diagnóstico e pressione **Reiniciar o radar**.

## 4. Veja funcionando

1. Abra [o app](app/) no Chrome ou Edge e pressione **Conectar USB**; escolha a porta chamada "USB JTAG/serial".
2. Escolha **Executar um cenário** → **Presença 3D e rastreamento**.
3. Ande na frente do radar. Sua posição aparece como um ponto azul.

Em seguida: [Wi-Fi de casa](wifi) para o uso diário, ou [Dados brutos](raw-data) para criar seus próprios algoritmos.

Volume, módulos pré-gravados ou firmware personalizado: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
