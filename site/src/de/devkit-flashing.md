---
title: Dev-Kit-Modul zum Flashen des Radars
nav: Dev-Kit-Flashen
group: Develop
---
# Dev-Kit-Modul zum Flashen des Radars

Die Radar-Base-Firmware gibt es in zwei Ausführungen, die aus demselben Quellcode gebaut werden:

| Ausführung | Was sie kann |
|---|---|
| Offen | Alles auf dieser Seite: startet jede Radar-App aus dem Speicher des Radars, JSON, Web-App, Wi-Fi, Smart Home |
| Dev-Kit | Offen + ein zusätzliches Modul, das den Flash des Radars beschreibt |

## Was das Modul tut

Wird eine App in den Flash des Radars geschrieben, startet sie beim Einschalten von selbst, ohne dass Radar Base sie
zuerst laden muss. Das Modul sichert vor jedem Schreiben den aktuellen Flash des Radars; schlägt die Sicherung fehl,
wird nichts geschrieben. Sicherungen lassen sich jederzeit über die App zurückspielen.

## Wie die Dev-Kit-Firmware geliefert wird

Die Dev-Kit-Firmware ist auf dem Dev-Kit-Board vorinstalliert. Updates werden den Käufern per E-Mail mitgeteilt; die Dev-Kit-Firmware ist kein öffentlicher Download. Die offene Firmware ist als Quellcode veröffentlicht: [github.com/Kymopsis/radar-base](https://github.com/Kymopsis/radar-base).

## Stapel-Flashen

Für viele Module nacheinander: Wählen Sie die Firmware einmal und schließen Sie dann ein Modul nach dem anderen an. Jedes Modul wird erkannt, auf Wunsch gesichert, geschrieben, geprüft, aus seinem Flash gestartet und nach seiner Version gefragt. Die Seite und die Board-LED zeigen PASS oder FAIL, ein Zähler zählt mit, und jedes Modul erhält eine Zeile in einem CSV-Protokoll, das Sie herunterladen können.

## Was es nicht tut

- Es verbindet sich nicht mit dem Internet.
- Es sammelt, speichert oder sendet keine Daten.
- Nichts anderes in der Firmware hängt von ihm ab.

Es ist geschlossen, weil das kleine Hilfsprogramm, das zum Schreiben ins Radar geladen wird, nicht von uns
veröffentlicht werden darf. Bei Dev-Kit-Geräten ist dieses Hilfsprogramm mit einem für jedes Gerät eigenen Schlüssel
verschlüsselt gespeichert.

## Ohne das Modul

Jede Radar-App läuft mit der offenen Firmware auch aus dem Speicher des Radars (Szenario starten). Es fehlt nur die
Bequemlichkeit „startet von selbst ohne Radar Base“. Die offene Firmware, die App und das JSON-Format sind von Grund auf offen angelegt; die App und das JSON-Format sind auf dieser Website dokumentiert.

Stückzahlen, vorgeflashte Module oder eigene Firmware: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
