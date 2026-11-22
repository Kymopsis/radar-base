---
title: Téléchargements
nav: Téléchargements
group: Develop
---
# Téléchargements

## Firmware

Le plus simple est le bouton **Installer le firmware** de la [page d'accueil](app) (Chrome ou Edge sur ordinateur, carte en USB). Les mêmes fichiers sont sur GitHub : [versions de Radar Base](https://github.com/Kymopsis/radar-base/releases).
Le firmware 0.9.2 et les suivants incluent les applis radar (présence 3D ; fréquence respiratoire et cardiaque).

## Radar Studio pour Windows

[Télécharger Radar Studio 0.9.1 pour Windows](https://github.com/Kymopsis/radar-base/releases/download/v0.9.1/KymopsisRadarStudio-0.9.1.exe)
(64 bits, environ 60 Mo, sans installation ; SHA-256 dans la
[version](https://github.com/Kymopsis/radar-base/releases/tag/v0.9.1)).

| Fonctionne avec | Fonctions |
|---|---|
| Tout Radar Base | Connexion en USB, vues par scénario, réglages courants, configuration maison connectée (MQTT) |
| Kit de développement Kymopsis Radar Base | Aussi données brutes, distance-Doppler, réglages de forme d'onde et de filtres, vos propres algorithmes, signes vitaux / HRV |

### « Windows a protégé votre PC »

Radar Studio n'est pas encore signé, donc Windows SmartScreen peut le bloquer la première fois. Choisissez **Informations complémentaires**, puis **Exécuter quand même**. Comparez le SHA-256 du fichier avec la page de la version si vous voulez être sûr qu'il s'agit de l'original.

## Code source

Firmware ouvert, appli web, composant ESPHome et bibliothèque Arduino : [github.com/Kymopsis/radar-base](https://github.com/Kymopsis/radar-base).

Volumes, modules préflashés ou firmware sur mesure : [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
