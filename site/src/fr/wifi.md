---
title: Mettre Radar Base sur votre Wi-Fi domestique
nav: Wi-Fi domestique
group: Connect
---
# Mettre Radar Base sur votre Wi-Fi domestique

Une fois Radar Base sur votre réseau domestique, tout téléphone, tablette ou ordinateur de la maison peut l'ouvrir sur
**http://kymradar.local**. Vos appareils restent sur votre Wi-Fi habituel.

## Configuration

Choisissez une méthode :

- **Depuis l'appli (USB ou point d'accès) :** Réglages de l'appareil → Wi-Fi domestique → choisissez votre réseau → saisissez le mot de passe → Connecter.
- **Depuis un terminal série :** `wifi <network-name> <password>` (sans espaces dans l'un comme dans l'autre).

Radar Base a besoin d'un réseau 2,4 GHz. La plupart des box proposent le 2,4 GHz et le 5 GHz sous le même nom, ce qui convient.

## L'ouvrir

- Sur ordinateurs, iPhone, iPad et Mac : **http://kymradar.local**
- Sur certains téléphones Android, les noms `.local` ne sont pas résolus. Utilisez l'adresse IP affichée sous Réglages de
  l'appareil → Wi-Fi domestique (par exemple http://192.168.1.57), ou cherchez l'appareil « kymradar » dans la liste de votre box.

## Ce qui arrive au point d'accès

Le point d'accès de Radar Base s'éteint de lui-même dès que votre Wi-Fi domestique fonctionne et qu'aucun téléphone
n'y est connecté. Il se rallume tout seul si le réseau domestique disparaît pendant 30 secondes - voir [Point d'accès de secours](hotspot).

## Changer ou supprimer le réseau

Réglages de l'appareil → Wi-Fi domestique → **Oublier le réseau**, ou sur la console `wifi forget`. Le point d'accès se rallume.

## Confidentialité

Radar Base ne dialogue qu'avec les appareils de votre propre réseau. Il n'a aucune connexion cloud et n'envoie aucune donnée nulle part.

Volumes, modules préflashés ou firmware sur mesure : [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
