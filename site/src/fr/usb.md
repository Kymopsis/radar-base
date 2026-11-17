---
title: Utiliser l'USB dans le navigateur
nav: USB dans le navigateur
group: Connect
---
# Utiliser l'USB dans le navigateur

L'appli Radar Base s'exécute dans Chrome ou Edge sur votre ordinateur et dialogue avec la carte par le câble USB
(Web Serial). Rien à installer, pas de Wi-Fi.

## Étapes

1. Connectez le port **USB** du contrôleur à l'ordinateur.
2. Ouvrez **[radar.kymopsis.com/app](app/)** dans Chrome ou Edge.
3. Appuyez sur **Connecter en USB** dans la barre du haut et choisissez « USB JTAG/serial debug unit ».
4. L'état passe au vert. Choisissez une tâche sur la page d'accueil.

Votre navigateur demande une fois par site l'autorisation d'utiliser le port ; rien d'autre sur votre ordinateur n'est partagé.

## Ce qui fonctionne en USB

| Fonction | En USB |
|---|---|
| Lancer un scénario, vues en direct, réglages du scénario | Oui |
| Réglages des données brutes et autotest de la liaison de données | Oui |
| Écrire le firmware dans le radar (kit de développement) | Oui |
| Configurer le Wi-Fi domestique | Oui (noms de réseau et mots de passe sans espaces) |
| Envoyer ou supprimer des fichiers firmware | Non - utilisez le Wi-Fi pour cela |

## Un seul programme à la fois

Un port USB ne peut être ouvert que dans un seul programme. Fermez tout terminal série avant de
connecter le navigateur, et inversement.

## Navigateurs

Web Serial est disponible dans Chrome, Edge et les autres navigateurs basés sur Chromium sur ordinateur. Safari et
Firefox ne le prennent pas en charge ; utilisez [Wi-Fi domestique](wifi) avec eux, ou sur téléphone.

Volumes, modules préflashés ou firmware sur mesure : [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
