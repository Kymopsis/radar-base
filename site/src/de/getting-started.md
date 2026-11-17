---
title: Erste Schritte
nav: Erste Schritte
group: Start
---
# Erste Schritte

Diese Anleitung führt Sie in etwa 15 Minuten vom Karton zu Live-Radarergebnissen.

## Was Sie brauchen

- Radar-Base-Controller (ESP32-S3-Board) und das 60-GHz-Radarmodul auf seinem Trägerboard
- 10 Steckbrücken (Buchse auf Buchse) - im Kit enthalten
- Zwei USB-Kabel: eines für Radar Base, eines für die Stromversorgung des Radar-Trägers
- Einen Computer mit Chrome oder Edge (Windows, macOS, Linux oder ChromeOS)

## 1. Radar-Träger vorbereiten

Entfernen Sie alle Jumper-Kappen von der seriellen Stiftleiste (J9) des Trägers und von der Boot-Stiftleiste
P19 / GND / P12. Sonst konkurriert der USB-Seriell-Chip des Trägers mit Radar Base auf denselben Leitungen.

## 2. Verdrahten

Schalten Sie zuerst alles aus. Verbinden Sie **zuerst GND**, dann den Rest. Die Bezeichnungen sind die auf dem
Radar-Träger aufgedruckten.

[[wiring]]

| Radar-Base-Pin | Bezeichnung am Radar-Träger | Zweck |
|---|---|---|
| GND | GND | Gemeinsame Masse (zuerst verbinden) |
| GPIO17 | RX0 | Befehle an die Radar-App |
| GPIO18 | TX0 | Ergebnisse der Radar-App |
| GPIO15 | RX1 | Apps ins Radar laden |
| GPIO16 | TX1 | Antworten beim Laden |
| GPIO4 | RST | Radar-Reset |
| GPIO5 | P12 | Boot-Auswahl (auch Datenauswahl) |
| GPIO6 | P19 | Boot-Auswahl |
| GPIO12 | SPI CLK | Takt für Rohdaten (nur für Rohdaten) |
| GPIO11 | SPI MOSI | Rohdaten (MISO nicht verwenden) |

Das Radar wird über seinen eigenen Träger versorgt (es zieht bis zu etwa 600 mA). Versorgen Sie es nie über den
3V3-Pin des Controller-Boards.

## 3. In der richtigen Reihenfolge einschalten

1. Stecken Sie den mit **USB** beschrifteten Port des Controllers in Ihren Computer. (Der mit UART beschriftete Port
   dient nur zum Aktualisieren der eigenen Firmware des Controllers.)
2. Versorgen Sie danach den Radar-Träger mit Strom.

Radar Base setzt die Boot-Pins des Radars gleich beim Start, deshalb muss das Radar als Zweites hochfahren. Falls Sie
die Reihenfolge vertauscht haben, öffnen Sie Diagnose und drücken **Radar neu starten**.

## 4. Es funktioniert

1. Öffnen Sie [die App](app/) in Chrome oder Edge und drücken Sie **USB verbinden**; wählen Sie den Port „USB JTAG/serial“.
2. Wählen Sie **Szenario starten** → **3D-Anwesenheit und Tracking**.
3. Gehen Sie vor dem Radar entlang. Ihre Position erscheint als blauer Punkt.

Weiter: [Heim-Wi-Fi](wifi) für den Alltag oder [Rohdaten](raw-data), um eigene Algorithmen zu bauen.

Stückzahlen, vorgeflashte Module oder eigene Firmware: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
