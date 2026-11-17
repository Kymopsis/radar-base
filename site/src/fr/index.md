---
title: Kymopsis Radar Base
nav: Vue d'ensemble
---
# Radar Base

Radar Base relie un module radar mmWave 60 GHz à votre ordinateur, à votre téléphone et à votre maison connectée.
Le travail radar est fait par le radar sur sa propre puce ; Radar Base y charge des applis radar, transforme leurs
résultats en un format JSON simple et les diffuse en USB et en Wi-Fi.

[[cards]]
- **Lancer une appli radar prête à l'emploi** | Positions des personnes en 3D, détection de chute, respiration et fréquence cardiaque. Sans programmer. | getting-started
- **L'utiliser dans le navigateur en USB** | Branchez, ouvrez ce site dans Chrome ou Edge, c'est tout. Pas de Wi-Fi nécessaire. | usb
- **Le mettre sur votre Wi-Fi domestique** | Accessible sur http://kymradar.local depuis n'importe quel téléphone ou ordinateur à la maison. | wifi
- **Créer votre propre intégration** | Lisez les résultats en JSON via USB ou WebSocket depuis n'importe quel programme. | json-api
[[/cards]]

## Matériel testé

| Matériel | État |
|---|---|
| Kit de développement Kymopsis Radar Base | Testé. Fonctionne dès la sortie de la boîte ; profil de carte inclus |
| Montage DIY Kymopsis : ESP32-S3 DevKit et le module radar 60 GHz sur sa carte porteuse, câblés comme dans [Prise en main](getting-started) | Testé. Fonctionne dès la sortie de la boîte ; profil de carte inclus |
| Autres cartes et modules radar | Non testé. Vérifiez vous-même l'attribution des broches et le profil de carte ; l'appli affiche alors « Carte inconnue, angles non calibrés » |

## Éditions ouverte et kit de développement

| Fonction | Firmware ouvert | Firmware du kit de développement |
|---|---|---|
| Exécuter des applis radar depuis la mémoire du radar, vues en direct | Oui | Oui |
| JSON via USB et Wi-Fi, appli web en 11 langues | Oui | Oui |
| Wi-Fi domestique, point d'accès de secours, Home Assistant (MQTT) | Oui | Oui |
| Flux de données brutes, réglages et autotest | Oui | Oui |
| Écrire une appli dans la flash du radar, avec sauvegarde | - | Oui |
| Flashage par lots : PASS / FAIL, compteur, journal CSV | - | Oui |
| Profil de carte défini en usine | - | Oui |

## Comment tout s'articule

[[diagram]]

| Élément | Ce qu'il fait |
|---|---|
| Module radar | Exécute les applis radar : chaque appli fait toute la chaîne de traitement du signal sur le radar et envoie des résultats |
| Radar Base (ESP32-S3) | Charge les applis radar, traduit les résultats en JSON, diffuse les données brutes, sert l'appli web, rejoint votre Wi-Fi |
| Appli web | Choisissez une tâche, suivez les résultats en direct, modifiez les réglages. La même appli en Wi-Fi et en USB, en 11 langues |
| Votre propre code | Lit les résultats JSON via USB ou WebSocket |
| Maison connectée | Home Assistant via MQTT (bientôt), ESPHome (plus tard) |

## Modes de connexion

| Mode | Quand l'utiliser | Guide |
|---|---|---|
| USB | Premiers pas et développement. Fonctionne partout, sans réseau | [USB dans le navigateur](usb) |
| Wi-Fi domestique | Usage quotidien depuis les téléphones et ordinateurs de la maison | [Wi-Fi domestique](wifi) |
| Point d'accès | Première configuration, ou quand aucun réseau domestique n'est joignable | [Point d'accès de secours](hotspot) |
| Home Assistant | Automatisations : lumières, alertes, tableaux de bord | [Home Assistant](home-assistant) |

Radar Base n'a jamais besoin d'un compte cloud. Vos données radar restent sur vos propres appareils.

Volumes, modules préflashés ou firmware sur mesure : [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
