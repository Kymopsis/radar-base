---
title: Use por USB no navegador
nav: USB no navegador
group: Connect
---
# Use por USB no navegador

O app do Radar Base roda no Chrome ou Edge no seu computador e conversa com a placa pelo cabo USB (Web Serial). Não há
nada para instalar e o Wi-Fi não é usado.

## Passos

1. Conecte a porta **USB** do controlador ao computador.
2. Abra **[radar.kymopsis.com/app](app/)** no Chrome ou Edge.
3. Pressione **Conectar USB** na barra superior e escolha "USB JTAG/serial debug unit".
4. O status fica verde. Escolha uma tarefa na página inicial.

O navegador pede permissão uma vez por site para usar a porta; nada mais do seu computador é compartilhado.

## O que funciona por USB

| Recurso | Por USB |
|---|---|
| Executar um cenário, visualizações ao vivo, configurações do cenário | Sim |
| Configurações de dados brutos e autoteste do link de dados | Sim |
| Gravar firmware no radar (kit de desenvolvimento) | Sim |
| Configurar o Wi-Fi de casa | Sim (nomes de rede e senhas sem espaços) |
| Enviar ou apagar arquivos de firmware | Não - use o Wi-Fi para isso |

## Só um programa por vez

Uma porta USB só pode estar aberta em um programa. Feche qualquer terminal serial antes de conectar o
navegador, e vice-versa.

## Navegadores

O Web Serial está disponível no Chrome, no Edge e em outros navegadores baseados em Chromium no desktop. Safari e
Firefox não o suportam; neles, ou em celulares, use o [Wi-Fi de casa](wifi).

Volume, módulos pré-gravados ou firmware personalizado: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
