---
title: Home Assistant
nav: Home Assistant
group: Smart home
---
# Home Assistant

Radar Base apparaît tout seul dans Home Assistant grâce à la découverte MQTT. Vous utilisez votre propre broker MQTT, en général
le module complémentaire **Mosquitto broker** de Home Assistant. Kymopsis n'exploite ni broker ni cloud ; rien ne quitte votre réseau.

## Ce qu'il vous faut

- Home Assistant avec le module complémentaire **Mosquitto broker** et l'intégration **MQTT**, ou tout autre broker MQTT que vous exploitez.
  Notez l'adresse du broker, le port (généralement 1883), l'utilisateur et le mot de passe.
- Un réseau Wi-Fi **2.4 GHz**, ou un réseau bi-bande où le 2.4 GHz et le 5 GHz partagent le même nom. L'ESP32-S3 n'a pas de
  radio 5 GHz.
- Firmware Radar Base 0.9.1 ou plus récent ([installez-le depuis le navigateur](app)).

## Mise en place

1. Rejoignez votre Wi-Fi : ouvrez l'appli Radar Base (en USB, ou via le point d'accès de la carte), Réglages de l'appareil, Wi-Fi.
2. Dans Réglages de l'appareil, Maison connectée : saisissez l'adresse du broker, le port, l'utilisateur et le mot de passe, puis activez MQTT.
3. Dans Home Assistant, ouvrez Paramètres → Appareils et services → MQTT. **Kymopsis Radar Base** y est avec ses entités.
   Rien d'autre à configurer.

## Entités

| Entité | Type | Unité / valeurs | Depuis l'appli radar |
|---|---|---|---|
| Occupation | Capteur binaire | actif / inactif | Présence 3D |
| Nombre de personnes | Capteur | nombre | Présence 3D |
| Personne la plus proche | Capteur | m | Présence 3D |
| Zone 1, Zone 2, Zone 3, Zone 4 | Capteur binaire | actif / inactif | Présence 3D |
| Chute détectée | Capteur binaire | actif / inactif | Détection de chute |
| Fréquence respiratoire | Capteur | /min | Respiration et fréquence cardiaque |
| Fréquence cardiaque | Capteur | /min | Respiration et fréquence cardiaque |
| Appli radar | Capteur | nom de l'appli en cours | toutes |
| Montage | Sélection | plafond / mur | Présence 3D |
| Redémarrer le radar | Bouton | | toutes |

Les valeurs que l'appli radar en cours ne signale pas restent vides ou inconnues. La fréquence respiratoire et cardiaque sont destinées uniquement au bien-être, pas à un usage médical.

## En ligne et hors ligne

La carte publie un topic de disponibilité. Quand vous désactivez MQTT sur la carte, l'appareil devient indisponible
aussitôt. Si la carte perd le courant ou le Wi-Fi, le broker la marque indisponible après environ 45 secondes ; elle revient d'elle-même quand la carte se reconnecte.

## Dépannage

| Problème | Que faire |
|---|---|
| L'appareil n'apparaît pas | Vérifiez la page Maison connectée de la carte : elle doit indiquer connecté. Vérifiez l'utilisateur et le mot de passe dans le broker. |
| La carte ne rejoint pas le Wi-Fi | Utilisez un réseau 2.4 GHz ou un réseau bi-bande au nom commun ; les réseaux 5 GHz seuls ne fonctionnent pas. |
| `kymradar.local` ne s'ouvre pas | Un VPN ou un proxy sur cet ordinateur peut prendre la main sur les noms `.local` ; désactivez-le, laissez `.local` passer en direct, ou ouvrez l'adresse IP de la carte. |
| Les distances s'affichent en pieds | Home Assistant utilise votre système d'unités : Paramètres → Système → Général → Système d'unités : Métrique. |

Volumes, modules préflashés ou firmware sur mesure : [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
