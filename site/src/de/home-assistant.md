---
title: Home Assistant
nav: Home Assistant
group: Smart home
---
# Home Assistant

> Kommt als Nächstes. Diese Seite beschreibt, wie die Integration funktionieren wird; sie wird gerade entwickelt.

Radar Base erscheint per MQTT-Discovery automatisch in Home Assistant. Sie nutzen Ihren eigenen MQTT-Broker; üblich ist
das Add-on **Mosquitto broker** in Home Assistant. Kymopsis betreibt weder einen Broker noch eine Cloud.

## Geplante Entitäten

| Entität | Typ | Quelle |
|---|---|---|
| Belegung | Binärsensor | 3D-Anwesenheit |
| Personenzahl | Sensor | 3D-Anwesenheit |
| Bereich 1-4 belegt | Binärsensoren | 3D-Anwesenheit |
| Sturz erkannt | Binärsensor und Ereignis | Sturzerkennung |
| Atemfrequenz, Herzfrequenz | Sensoren (Diagnose) | Atmung und Herzfrequenz |
| Montage (Decke / Wand) | Auswahl | 3D-Anwesenheit |

## Geplante Einrichtung

1. Installieren Sie in Home Assistant das Mosquitto-broker-Add-on und die MQTT-Integration.
2. In der Radar-Base-App: Geräteeinstellungen → Smart Home → Broker-Adresse, Benutzer und Passwort eingeben.
3. Radar Base erscheint unter Einstellungen → Geräte.

Atem- und Herzfrequenz dienen nur dem Wohlbefinden und nicht der medizinischen Nutzung.

Stückzahlen, vorgeflashte Module oder eigene Firmware: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
