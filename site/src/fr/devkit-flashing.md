---
title: Module de flashage du radar (kit de développement)
nav: Flashage du kit de développement
group: Develop
---
# Module de flashage du radar (kit de développement)

Le firmware de Radar Base existe en deux éditions, construites à partir du même code source :

| Édition | Ce qu'elle sait faire |
|---|---|
| Ouverte | Tout ce qui est sur ce site : exécute chaque appli radar depuis la mémoire du radar, JSON, appli web, Wi-Fi, maison connectée |
| Kit de développement | Ouverte + un module supplémentaire qui écrit dans la flash du radar |

## Ce que fait le module

Une appli écrite dans la flash du radar démarre toute seule à la mise sous tension, sans que Radar Base ait besoin de la
charger d'abord. Le module sauvegarde la flash actuelle du radar avant chaque écriture ; si la sauvegarde échoue, rien
n'est écrit. Les sauvegardes peuvent être restaurées à tout moment depuis l'appli.

## Comment le firmware du kit de développement est livré

Le firmware du kit de développement est préchargé sur la carte du kit. Les mises à jour sont annoncées aux acheteurs par e-mail ; ce firmware n'est pas un téléchargement public. Le firmware ouvert est publié sous forme de code source : [github.com/Kymopsis/radar-base](https://github.com/Kymopsis/radar-base).

## Flashage par lots

Pour de nombreux modules à la suite : choisissez le firmware une seule fois, puis branchez les modules un par un. Chaque module est détecté, sauvegardé si vous le souhaitez, écrit, vérifié, démarré depuis sa flash et interrogé sur sa version. La page et le voyant de la carte affichent PASS ou FAIL, un compteur s'incrémente, et chaque module reçoit une ligne dans un journal CSV que vous pouvez télécharger.

## Ce qu'il ne fait pas

- Il ne se connecte pas à Internet.
- Il ne collecte, ne stocke et n'envoie aucune donnée.
- Rien d'autre dans le firmware n'en dépend.

Il est fermé parce que le petit programme auxiliaire qu'il charge dans le radar pour écrire n'est pas à nous de le
publier. Sur les unités du kit de développement, ce programme est stocké chiffré avec une clé propre à chaque unité.

## Sans ce module

Chaque appli radar s'exécute aussi depuis la mémoire du radar avec le firmware ouvert (Lancer un scénario). Vous perdez
seulement le confort du « démarrage automatique sans Radar Base ». Le firmware ouvert, l'appli et le format JSON sont ouverts par conception ; l'appli et le format JSON sont documentés sur ce site.

Volumes, modules préflashés ou firmware sur mesure : [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
