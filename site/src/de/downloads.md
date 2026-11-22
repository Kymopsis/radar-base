---
title: Downloads
nav: Downloads
group: Develop
---
# Downloads

## Firmware

Am einfachsten geht es mit der Schaltfläche **Firmware installieren** auf der [Startseite](app) (Chrome oder Edge am Desktop, Board per USB). Dieselben Dateien liegen auch auf GitHub: [Radar Base Releases](https://github.com/Kymopsis/radar-base/releases).
Firmware 0.9.2 und neuer enthält die Radar-Apps (3D-Anwesenheit; Atem- und Herzfrequenz).

## Radar Studio für Windows

[Radar Studio 0.9.1 für Windows herunterladen](https://github.com/Kymopsis/radar-base/releases/download/v0.9.1/KymopsisRadarStudio-0.9.1.exe)
(64 Bit, etwa 60 MB, keine Installation nötig; SHA-256 im
[Release](https://github.com/Kymopsis/radar-base/releases/tag/v0.9.1)).

| Funktioniert mit | Funktionen |
|---|---|
| Jedes Radar Base | Per USB verbinden, Szenarioansichten, häufige Einstellungen, Smart-Home-Einrichtung (MQTT) |
| Kymopsis Radar Base Dev-Kit | Zusätzlich Rohdaten, Range-Doppler, Wellenform- und Filtereinstellungen, eigene Algorithmen, Vitalzeichen / HRV |

### „Der Computer wurde durch Windows geschützt“

Radar Studio ist noch nicht code-signiert, deshalb kann Windows SmartScreen es beim ersten Mal stoppen. Wählen Sie **Weitere Informationen** und dann **Trotzdem ausführen**. Prüfen Sie den SHA-256 der Datei anhand der Release-Seite, wenn Sie sichergehen wollen, dass es das Original ist.

## Quellcode

Offene Firmware, Web-App, ESPHome-Komponente und Arduino-Bibliothek: [github.com/Kymopsis/radar-base](https://github.com/Kymopsis/radar-base).

Stückzahlen, vorgeflashte Module oder eigene Firmware: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
