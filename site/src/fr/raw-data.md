---
title: Données brutes et vos propres algorithmes
nav: Données brutes
group: Develop
---
# Données brutes et vos propres algorithmes

Radar Base peut diffuser les données brutes du radar (chaque chirp, les deux voies de réception) en USB. Le firmware ouvert inclut ce flux et ses réglages  ; Radar Studio pour Windows les affiche et les traite, et ouvre ces vues avec un kit de développement ([Téléchargements](downloads)).

## Ce qu'apporte le firmware ouvert

| Fonction | Où |
|---|---|
| Réglages des données brutes : chirp, gain, filtres, antennes | Appli → Données brutes et réglages du radar, ou console `cfg <key> <value>` |
| Autotest de la liaison de données : chaque bloc est vérifié | Appli → Données brutes et réglages du radar → **Lancer le test** |
| Résultats en JSON pour votre propre code | [Résultats JSON et API](json-api) |

## Où exécuter un algorithme

Prototypez sur le PC. Vérifiez ensuite si le matériel FFT et CFAR du radar peut faire le travail ; si oui, il doit
s'exécuter sur le radar et seuls les résultats en sortent. Sinon, regardez l'ESP32, et ne le gardez sur le PC que si
aucun des deux ne convient.

## Vérifier la liaison de données

Appli → Données brutes et réglages du radar → **Lancer le test**. Il exécute l'appli de données brutes pendant quelques
secondes et vérifie chaque bloc : les blocs corrompus et manquants doivent tous deux être à 0.

Volumes, modules préflashés ou firmware sur mesure : [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
