---
title: Rohdaten und eigene Algorithmen
nav: Rohdaten
group: Develop
---
# Rohdaten und eigene Algorithmen

Radar Base kann die rohen Daten des Radars (jeden Chirp, beide Empfangskanäle) über USB streamen. Die offene Firmware enthält diesen Datenstrom und seine Einstellungen; Radar Studio für Windows zeigt und verarbeitet sie und öffnet diese Ansichten mit einem Dev-Kit ([Downloads](downloads)).

## Was die offene Firmware bietet

| Funktion | Wo |
|---|---|
| Rohdaten-Einstellungen: Chirp, Verstärkung, Filter, Antennen | App → Rohdaten und Radar-Einstellungen oder Konsole `cfg <key> <value>` |
| Selbsttest der Datenverbindung: jeder Block wird geprüft | App → Rohdaten und Radar-Einstellungen → **Test starten** |
| Ergebnisse als JSON für Ihren eigenen Code | [JSON-Ergebnisse und API](json-api) |

## Wo ein Algorithmus laufen sollte

Prototypen entstehen am PC. Prüfen Sie dann, ob die FFT- und CFAR-Hardware des Radars die Aufgabe übernehmen kann; wenn
ja, sollte der Algorithmus im Radar laufen und nur Ergebnisse verlassen es. Andernfalls prüfen Sie den ESP32 und lassen
ihn nur am PC laufen, wenn keines von beiden reicht.

## Datenverbindung prüfen

App → Rohdaten und Radar-Einstellungen → **Test starten**. Er führt die Rohdaten-App einige Sekunden lang aus und prüft
jeden Block: Beschädigte und fehlende Blöcke sollten beide 0 sein.

Stückzahlen, vorgeflashte Module oder eigene Firmware: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
