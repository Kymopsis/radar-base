---
title: Home Assistant
nav: Home Assistant
group: Smart home
---
# Home Assistant

O Radar Base aparece sozinho no Home Assistant por meio da descoberta MQTT. Você usa o seu próprio broker MQTT, normalmente
o complemento **Mosquitto broker** do Home Assistant. A Kymopsis não opera broker nem nuvem; nada sai da sua rede.

## O que você precisa

- Home Assistant com o complemento **Mosquitto broker** e a integração **MQTT**, ou qualquer outro broker MQTT que você mesmo execute.
  Anote o endereço do broker, a porta (normalmente 1883), o usuário e a senha.
- Uma rede Wi-Fi de **2.4 GHz**, ou uma rede dual-band em que 2.4 GHz e 5 GHz têm o mesmo nome. O ESP32-S3 não tem
  rádio de 5 GHz.
- Firmware do Radar Base 0.9.1 ou posterior ([instale pelo navegador](app)).

## Configuração

1. Conecte-se ao seu Wi-Fi: abra o app do Radar Base (por USB ou pelo hotspot da placa), Configurações do dispositivo, Wi-Fi.
2. Em Configurações do dispositivo, Casa inteligente: informe o endereço do broker, a porta, o usuário e a senha, e ative o MQTT.
3. No Home Assistant, abra Configurações → Dispositivos e serviços → MQTT. O **Kymopsis Radar Base** está lá com suas entidades.
   Não há mais nada para configurar.

## Entidades

| Entidade | Tipo | Unidade / valores | Do app de radar |
|---|---|---|---|
| Ocupação | Sensor binário | ligado / desligado | Presença 3D |
| Número de pessoas | Sensor | número | Presença 3D |
| Pessoa mais próxima | Sensor | m | Presença 3D |
| Área 1, Área 2, Área 3, Área 4 | Sensor binário | ligado / desligado | Presença 3D |
| Queda detectada | Sensor binário | ligado / desligado | Detecção de quedas |
| Frequência respiratória | Sensor | /min | Respiração e frequência cardíaca |
| Frequência cardíaca | Sensor | /min | Respiração e frequência cardíaca |
| App de radar | Sensor | nome do app em execução | qualquer |
| Montagem | Seleção | teto / parede | Presença 3D |
| Reiniciar radar | Botão | | qualquer |

Os valores que o app de radar em execução não informa ficam vazios ou desconhecidos. A frequência respiratória e a cardíaca são apenas para bem-estar, não para uso médico.

## Online e offline

A placa publica um tópico de disponibilidade. Quando você desativa o MQTT na placa, o dispositivo fica indisponível
na hora. Se a placa perder energia ou Wi-Fi, o broker a marca como indisponível após cerca de 45 segundos; ela volta sozinha quando a placa se reconecta.

## Solução de problemas

| Problema | O que fazer |
|---|---|
| O dispositivo não aparece | Veja a página Casa inteligente da placa: ela deve indicar conectado. Confira o usuário e a senha no broker. |
| A placa não entra no Wi-Fi | Use uma rede de 2.4 GHz ou dual-band com um nome em comum; redes apenas de 5 GHz não funcionam. |
| `kymradar.local` não abre | Uma VPN ou um proxy nesse computador pode tomar conta dos nomes `.local`; desligue-o, deixe `.local` ir direto ou abra o endereço IP da placa. |
| As distâncias aparecem em pés | O Home Assistant usa o seu sistema de unidades: Configurações → Sistema → Geral → Sistema de unidades: Métrico. |

Volume, módulos pré-gravados ou firmware personalizado: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
