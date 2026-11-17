---
title: Kymopsis Radar Base
nav: Übersicht
---
# Radar Base

Radar Base verbindet ein 60-GHz-mmWave-Radarmodul mit Ihrem Computer, Ihrem Handy und Ihrem Smart Home.
Die Radararbeit erledigt das Radar auf seinem eigenen Chip; Radar Base lädt Radar-Apps hinein, wandelt deren
Ergebnisse in ein einfaches JSON-Format um und stellt sie über USB und Wi-Fi bereit.

[[cards]]
- **Fertige Radar-App starten** | Personenpositionen in 3D, Sturzerkennung, Atem- und Herzfrequenz. Ohne Programmieren. | getting-started
- **Im Browser über USB nutzen** | Einstecken, diese Seite in Chrome oder Edge öffnen, fertig. Kein Wi-Fi nötig. | usb
- **Ins Heim-Wi-Fi bringen** | Erreichbar unter http://kymradar.local von jedem Handy und Computer zu Hause. | wifi
- **Eigene Integration bauen** | Die Ergebnisse als JSON über USB oder WebSocket aus jedem Programm lesen. | json-api
[[/cards]]

## Getestete Hardware

| Hardware | Status |
|---|---|
| Kymopsis-Radar-Base-Dev-Kit | Getestet. Funktioniert direkt; Board-Profil enthalten |
| Kymopsis-DIY-Aufbau: ESP32-S3 DevKit und das 60-GHz-Radarmodul auf seinem Träger, verdrahtet wie in [Erste Schritte](getting-started) | Getestet. Funktioniert direkt; Board-Profil enthalten |
| Andere Boards und Radarmodule | Ungetestet. Prüfen Sie Pinbelegung und Board-Profil selbst; die App zeigt dann „Unbekanntes Board, Winkel nicht kalibriert“ |

## Offene Edition und Dev-Kit-Edition

| Funktion | Offene Firmware | Dev-Kit-Firmware |
|---|---|---|
| Radar-Apps aus dem Speicher des Radars ausführen, Live-Ansichten | Ja | Ja |
| JSON über USB und Wi-Fi, Web-App in 11 Sprachen | Ja | Ja |
| Heim-Wi-Fi, Hotspot-Rückfall, Home Assistant (MQTT) | Ja | Ja |
| Rohdatenstrom, Einstellungen und Selbsttest | Ja | Ja |
| App in den eigenen Flash des Radars schreiben, mit Sicherung | - | Ja |
| Stapel-Flashen: PASS / FAIL, Zähler, CSV-Protokoll | - | Ja |
| Board-Profil ab Werk eingestellt | - | Ja |

## So passt alles zusammen

[[diagram]]

| Teil | Was es tut |
|---|---|
| Radarmodul | Führt die Radar-Apps aus: Jede App erledigt die ganze Signalkette im Radar und meldet Ergebnisse |
| Radar Base (ESP32-S3) | Lädt Radar-Apps, übersetzt Ergebnisse in JSON, streamt Rohdaten, stellt die Web-App bereit, verbindet sich mit Ihrem Wi-Fi |
| Web-App | Aufgabe wählen, Live-Ergebnisse ansehen, Einstellungen ändern. Dieselbe App über Wi-Fi und USB, in 11 Sprachen |
| Ihr eigener Code | Liest die JSON-Ergebnisse über USB oder WebSocket |
| Smart Home | Home Assistant über MQTT (kommt als Nächstes), ESPHome (später) |

## Verbindungsarten

| Modus | Wann verwenden | Anleitung |
|---|---|---|
| USB | Erste Schritte und Entwicklung. Funktioniert überall, ohne Netzwerk | [USB im Browser](usb) |
| Heim-Wi-Fi | Alltag mit Handys und Computern zu Hause | [Heim-Wi-Fi](wifi) |
| Hotspot | Ersteinrichtung oder wenn kein Heimnetz erreichbar ist | [Hotspot als Rückfall](hotspot) |
| Home Assistant | Automationen: Licht, Warnungen, Dashboards | [Home Assistant](home-assistant) |

Radar Base braucht nie ein Cloud-Konto. Ihre Radardaten bleiben auf Ihren eigenen Geräten.

Stückzahlen, vorgeflashte Module oder eigene Firmware: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
