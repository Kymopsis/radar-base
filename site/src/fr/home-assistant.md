---
title: Home Assistant
nav: Home Assistant
group: Smart home
---
# Home Assistant

> Bientôt. Cette page décrit le fonctionnement prévu de l'intégration ; elle est en cours de développement.

Radar Base apparaîtra tout seul dans Home Assistant grâce à la découverte MQTT. Vous utilisez votre propre broker MQTT ;
le choix habituel est le module complémentaire **Mosquitto broker** de Home Assistant. Kymopsis n'exploite ni broker ni cloud.

## Entités prévues

| Entité | Type | Source |
|---|---|---|
| Occupation | Capteur binaire | Présence 3D |
| Nombre de personnes | Capteur | Présence 3D |
| Zone 1-4 occupée | Capteurs binaires | Présence 3D |
| Chute détectée | Capteur binaire et événement | Détection de chute |
| Fréquence respiratoire, fréquence cardiaque | Capteurs (diagnostic) | Respiration et fréquence cardiaque |
| Montage (plafond / mur) | Sélection | Présence 3D |

## Installation prévue

1. Dans Home Assistant, installez le module complémentaire Mosquitto broker et l'intégration MQTT.
2. Dans l'appli Radar Base : Réglages de l'appareil → Maison connectée → saisissez l'adresse du broker, l'utilisateur et le mot de passe.
3. Radar Base apparaît sous Paramètres → Appareils.

La fréquence respiratoire et cardiaque sont destinées uniquement au bien-être, pas à un usage médical.

Volumes, modules préflashés ou firmware sur mesure : [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
