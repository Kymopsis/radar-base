---
title: Über USB im Browser nutzen
nav: USB im Browser
group: Connect
---
# Über USB im Browser nutzen

Die Radar-Base-App läuft in Chrome oder Edge auf Ihrem Computer und spricht über das USB-Kabel (Web Serial) mit dem
Board. Nichts zu installieren, kein Wi-Fi beteiligt.

## Schritte

1. Verbinden Sie den **USB**-Port des Controllers mit dem Computer.
2. Öffnen Sie **[radar.kymopsis.com/app](app/)** in Chrome oder Edge.
3. Drücken Sie oben in der Leiste **USB verbinden** und wählen Sie „USB JTAG/serial debug unit“.
4. Der Status wird grün. Wählen Sie auf der Startseite eine Aufgabe.

Ihr Browser fragt pro Website einmal nach der Erlaubnis, den Port zu nutzen; sonst wird nichts von Ihrem Computer
freigegeben.

## Was über USB funktioniert

| Funktion | Über USB |
|---|---|
| Szenario starten, Live-Ansichten, Szenario-Einstellungen | Ja |
| Rohdaten-Einstellungen und Selbsttest der Datenverbindung | Ja |
| Firmware ins Radar schreiben (Dev-Kit) | Ja |
| Heim-Wi-Fi einrichten | Ja (Netzwerknamen und Passwörter ohne Leerzeichen) |
| Firmware-Dateien hochladen oder löschen | Nein - dafür Wi-Fi verwenden |

## Nur ein Programm gleichzeitig

Ein USB-Port kann nur in einem Programm geöffnet sein. Schließen Sie jedes serielle Terminal, bevor
Sie den Browser verbinden, und umgekehrt.

## Browser

Web Serial gibt es in Chrome, Edge und anderen Chromium-basierten Browsern auf dem Desktop. Safari und Firefox
unterstützen es nicht; nutzen Sie dort oder auf Handys [Heim-Wi-Fi](wifi).

Stückzahlen, vorgeflashte Module oder eigene Firmware: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
