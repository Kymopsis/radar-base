---
title: FAQ und Fehlersuche
nav: FAQ
group: Help
---
# FAQ und Fehlersuche

## Die App verbindet sich, zeigt aber keine Ergebnisse

- Hat der Radar-Träger Strom? Wurde Radar Base zuerst eingeschaltet? Öffnen Sie Diagnose → **Radar neu starten**.
- Diagnose → **Radar nach Version fragen**: Kommt eine Antwort, sind die Ergebnisleitungen (RX0/TX0) in Ordnung.

## Ein Szenario lässt sich nicht laden

- Diagnose → **Boot-Verbindung prüfen**. Keine Antwort deutet meist auf eine lose Steckbrücke an RX1, TX1, RST, P12 oder P19 hin.
- Stecken Sie die Kabel neu; Steckbrücken verlieren leicht den Kontakt. Halten Sie sie unter 15 cm.

## Der Rohdaten-Selbsttest zeigt beschädigte oder fehlende Blöcke

- Prüfen Sie die Leitungen SPI CLK und MOSI (MOSI, nicht MISO) und legen Sie ein Massekabel daneben.
- Senken Sie die SPI-Geschwindigkeit: Konsole `cfg spi 12.5`.

## Jeder Block ist fehlerhaft und enthält nur 00 oder FF

Die Datenleitung hängt am MISO-Pin des Trägers. Stecken Sie sie auf **MOSI** um.

## Der Browser listet den USB-Port nicht auf

- Verwenden Sie den mit **USB** beschrifteten Port am Controller, nicht UART.
- Schließen Sie jedes serielle Terminal: Nur ein Programm kann den Port nutzen.
- Verwenden Sie Chrome oder Edge auf einem Desktop-Computer.

## http://kymradar.local lässt sich nicht öffnen

Manche Android-Handys lösen `.local`-Namen nicht auf. Verwenden Sie die IP-Adresse unter Geräteeinstellungen → Heim-Wi-Fi.

## Atem- oder Herzfrequenz bleibt leer

Sitzen oder liegen Sie ruhig 0,5-1,5 m vor dem Radar, die Brust ihm zugewandt, und warten Sie etwa 20 Sekunden. Diese
Messwerte dienen nur dem Wohlbefinden und nicht der medizinischen Nutzung.

## Eine Person wird angezeigt, der Bereich leuchtet aber nicht (oder umgekehrt)

Bereiche und Tracking sind getrennte Radar-Ausgaben. Eine sehr still sitzende Person kann aus dem Tracking fallen,
während der Bereich belegt bleibt. Die App zeigt die höchste Anzahl des letzten Moments, damit die Zahl stabil bleibt.

Stückzahlen, vorgeflashte Module oder eigene Firmware: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
