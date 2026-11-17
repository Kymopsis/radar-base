---
title: FAQ et dépannage
nav: FAQ
group: Help
---
# FAQ et dépannage

## L'appli se connecte mais n'affiche aucun résultat

- La carte porteuse du radar est-elle alimentée ? Radar Base a-t-il été alimenté en premier ? Ouvrez Diagnostic → **Redémarrer le radar**.
- Diagnostic → **Demander sa version au radar** : une réponse signifie que les fils de résultats (RX0/TX0) sont bons.

## Le chargement d'un scénario échoue

- Diagnostic → **Vérifier la liaison de démarrage**. Pas de réponse : le plus souvent un fil volant mal enfoncé sur RX1, TX1, RST, P12 ou P19.
- Réenfoncez les fils ; les fils volants perdent facilement le contact. Gardez-les à moins de 15 cm.

## L'autotest des données brutes signale des blocs corrompus ou manquants

- Vérifiez les fils SPI CLK et MOSI (MOSI, pas MISO) et placez un fil de masse à côté.
- Baissez la vitesse SPI : console `cfg spi 12.5`.

## Tous les blocs sont mauvais et ne contiennent que 00 ou FF

Le fil de données est sur la broche MISO de la carte porteuse. Déplacez-le sur **MOSI**.

## Le navigateur n'affiche pas le port USB

- Utilisez le port marqué **USB** du contrôleur, pas UART.
- Fermez tout terminal série : un seul programme peut utiliser le port.
- Utilisez Chrome ou Edge sur un ordinateur de bureau.

## http://kymradar.local ne s'ouvre pas

Certains téléphones Android ne résolvent pas les noms en `.local`. Utilisez l'adresse IP affichée sous Réglages de l'appareil → Wi-Fi domestique.

## La fréquence respiratoire ou cardiaque reste vide

Asseyez-vous ou allongez-vous sans bouger à 0,5-1,5 m devant le radar, la poitrine face à lui, et attendez environ
20 secondes. Ces mesures sont destinées uniquement au bien-être, pas à un usage médical.

## Une personne s'affiche mais la zone ne s'allume pas (ou l'inverse)

Les zones et le suivi sont deux sorties radar distinctes. Une personne très immobile peut sortir du suivi alors que la
zone reste occupée. L'appli affiche le nombre le plus élevé du dernier instant pour garder un chiffre stable.

Volumes, modules préflashés ou firmware sur mesure : [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
