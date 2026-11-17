---
title: Perguntas frequentes e solução de problemas
nav: Perguntas frequentes
group: Help
---
# Perguntas frequentes e solução de problemas

## O app conecta, mas não mostra resultados

- A placa do radar está alimentada? O Radar Base foi ligado primeiro? Abra Diagnóstico → **Reiniciar o radar**.
- Diagnóstico → **Perguntar a versão ao radar**: se houver resposta, os fios de resultados (RX0/TX0) estão bons.

## O carregamento de um cenário falha

- Diagnóstico → **Verificar o link de inicialização**. Sem resposta, normalmente há um fio solto em RX1, TX1, RST, P12 ou P19.
- Encaixe os fios de novo; jumpers perdem contato com facilidade. Mantenha-os com menos de 15 cm.

## O autoteste de dados brutos mostra blocos corrompidos ou ausentes

- Verifique os fios SPI CLK e MOSI (MOSI, não MISO) e coloque um fio de terra ao lado deles.
- Reduza a velocidade SPI: console `cfg spi 12.5`.

## Todos os blocos estão ruins e contêm apenas 00 ou FF

O fio de dados está no pino MISO da placa. Mude-o para **MOSI**.

## O navegador não lista a porta USB

- Use a porta marcada como **USB** no controlador, não UART.
- Feche qualquer terminal serial: só um programa pode usar a porta.
- Use Chrome ou Edge em um computador de mesa.

## http://kymradar.local não abre

Alguns celulares Android não resolvem nomes `.local`. Use o endereço IP mostrado em Configurações do dispositivo → Wi-Fi de casa.

## A frequência respiratória ou cardíaca continua vazia

Sente-se ou deite-se parado a 0,5-1,5 m à frente do radar, com o peito voltado para ele, e espere cerca de 20 segundos.
Essas medições são apenas para bem-estar, não para uso médico.

## Uma pessoa aparece, mas a área não acende (ou o contrário)

Áreas e rastreamento são saídas separadas do radar. Uma pessoa muito parada pode sair do rastreamento enquanto a área
continua ocupada. O app mostra a maior contagem do último momento para manter o número estável.

Volume, módulos pré-gravados ou firmware personalizado: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
