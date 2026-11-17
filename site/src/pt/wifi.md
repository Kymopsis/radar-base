---
title: Coloque o Radar Base no seu Wi-Fi de casa
nav: Wi-Fi de casa
group: Connect
---
# Coloque o Radar Base no seu Wi-Fi de casa

Quando o Radar Base está na sua rede doméstica, qualquer celular, tablet ou computador em casa pode abri-lo em
**http://kymradar.local**. Seus dispositivos continuam no Wi-Fi normal.

## Configuração

Escolha um jeito:

- **Pelo app (USB ou hotspot):** Configurações do dispositivo → Wi-Fi de casa → escolha sua rede → digite a senha → Conectar.
- **Por um terminal serial:** `wifi <network-name> <password>` (sem espaços em nenhum dos dois).

O Radar Base precisa de uma rede de 2,4 GHz. A maioria dos roteadores domésticos oferece 2,4 GHz e 5 GHz com o mesmo nome, e isso funciona bem.

## Abra

- Em computadores, iPhones, iPads e Macs: **http://kymradar.local**
- Em alguns celulares Android os nomes `.local` não são resolvidos. Use o endereço IP mostrado em Configurações do
  dispositivo → Wi-Fi de casa (por exemplo http://192.168.1.57), ou procure o dispositivo "kymradar" na lista do seu roteador.

## O que acontece com o hotspot

O hotspot do Radar Base se desliga sozinho quando o seu Wi-Fi de casa funciona e nenhum celular está conectado a ele. Ele
volta a ligar sozinho se a rede doméstica sumir por 30 segundos - veja [Hotspot de reserva](hotspot).

## Alterar ou remover a rede

Configurações do dispositivo → Wi-Fi de casa → **Esquecer rede**, ou no console `wifi forget`. O hotspot volta a ligar.

## Privacidade

O Radar Base só conversa com dispositivos da sua própria rede. Ele não tem conexão com a nuvem e não envia dados para lugar nenhum.

Volume, módulos pré-gravados ou firmware personalizado: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
