---
title: Módulo de gravação do radar (kit de desenvolvimento)
nav: Gravação do kit de desenvolvimento
group: Develop
---
# Módulo de gravação do radar (kit de desenvolvimento)

O firmware do Radar Base vem em duas edições, criadas a partir do mesmo código-fonte:

| Edição | O que ela faz |
|---|---|
| Aberta | Tudo o que está neste site: executa qualquer app do radar a partir da memória do radar, JSON, app web, Wi-Fi, casa inteligente |
| Kit de desenvolvimento | Aberta + um módulo extra que grava na flash do radar |

## O que o módulo faz

Quando um app é gravado na flash do radar, ele inicia sozinho ao ligar, sem que o Radar Base precise carregá-lo antes. O
módulo faz um backup da flash atual do radar antes de cada gravação; se o backup falhar, nada é gravado. Os backups
podem ser restaurados a qualquer momento pelo app.

## Como o firmware do kit de desenvolvimento é entregue

O firmware do kit de desenvolvimento vem pré-carregado na placa do kit. As atualizações são comunicadas aos compradores por e-mail; esse firmware não é um download público. O firmware aberto será publicado como código-fonte em um repositório público; esta página trará o link quando ele sair.

## Gravação em lote

Para muitos módulos em sequência: escolha o firmware uma vez e conecte um módulo depois do outro. Cada módulo é detectado, copiado se você quiser, gravado, verificado, iniciado a partir da sua flash e consultado sobre a sua versão. A página e a luz da placa mostram PASS ou FAIL, um contador vai somando e cada módulo recebe uma linha em um registro CSV que você pode baixar.

## O que ele não faz

- Não se conecta à internet.
- Não coleta, armazena nem envia nenhum dado.
- Nada mais no firmware depende dele.

Ele é fechado porque o pequeno programa auxiliar que ele carrega no radar para gravar não é nosso para publicar. Nas
unidades do kit de desenvolvimento, esse programa fica armazenado criptografado com uma chave exclusiva de cada unidade.

## Sem ele

Todo app do radar também roda a partir da memória do radar com o firmware aberto (Executar um cenário). Você só perde a
comodidade de "inicia sozinho sem o Radar Base". O firmware aberto, o app e o formato JSON são abertos por concepção; o app e o formato JSON estão documentados neste site.

Volume, módulos pré-gravados ou firmware personalizado: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
