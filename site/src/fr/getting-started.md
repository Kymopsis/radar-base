---
title: Premiers pas
nav: Premiers pas
group: Start
---
# Premiers pas

Ce guide vous mène de la boîte aux résultats radar en direct en 15 minutes environ.

## Ce qu'il vous faut

- Le contrôleur Radar Base (carte ESP32-S3) et le module radar 60 GHz sur sa carte porteuse
- 10 fils volants (femelle-femelle) - inclus dans le kit
- Deux câbles USB : un pour Radar Base, un pour alimenter la carte porteuse du radar
- Un ordinateur avec Chrome ou Edge (Windows, macOS, Linux ou ChromeOS)

## 1. Préparer la carte porteuse du radar

Retirez tous les cavaliers de l'embase série de la carte (J9) et de l'embase de démarrage P19 / GND / P12.
Sinon, la puce USB-série de la carte entre en concurrence avec Radar Base sur les mêmes fils.

## 2. Câbler

Éteignez tout d'abord. Connectez **GND en premier**, puis le reste. Les repères sont ceux imprimés sur la carte porteuse
du radar.

[[wiring]]

| Broche de Radar Base | Repère de la carte du radar | Rôle |
|---|---|---|
| GND | GND | Masse commune (à connecter en premier) |
| GPIO17 | RX0 | Commandes vers l'appli radar |
| GPIO18 | TX0 | Résultats de l'appli radar |
| GPIO15 | RX1 | Chargement des applis dans le radar |
| GPIO16 | TX1 | Réponses pendant le chargement |
| GPIO4 | RST | Réinitialisation du radar |
| GPIO5 | P12 | Sélection du démarrage (aussi sélection des données) |
| GPIO6 | P19 | Sélection du démarrage |
| GPIO12 | SPI CLK | Horloge des données brutes (usage données brutes uniquement) |
| GPIO11 | SPI MOSI | Données brutes (n'utilisez pas MISO) |

Le radar est alimenté par sa propre carte porteuse (il consomme jusqu'à 600 mA environ). Ne l'alimentez jamais depuis la
broche 3V3 de la carte du contrôleur.

## 3. Allumer dans le bon ordre

1. Branchez le port du contrôleur marqué **USB** à votre ordinateur. (Le port marqué UART ne sert qu'à mettre à jour le
   firmware du contrôleur lui-même.)
2. Alimentez ensuite la carte porteuse du radar.

Radar Base configure les broches de démarrage du radar dès qu'il démarre, le radar doit donc démarrer en second. Si vous
les avez alimentés dans l'autre sens, ouvrez Diagnostic et appuyez sur **Redémarrer le radar**.

## 4. Voir que ça marche

1. Ouvrez [l'appli](app/) dans Chrome ou Edge et appuyez sur **Connecter en USB** ; choisissez le port nommé « USB JTAG/serial ».
2. Choisissez **Lancer un scénario** → **Présence 3D et suivi**.
3. Marchez devant le radar. Votre position apparaît sous forme de point bleu.

Ensuite : [Wi-Fi domestique](wifi) pour l'usage quotidien, ou [Données brutes](raw-data) pour créer vos propres algorithmes.

Volumes, modules préflashés ou firmware sur mesure : [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
