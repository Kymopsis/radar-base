---
title: FAQ e risoluzione dei problemi
nav: FAQ
group: Help
---
# FAQ e risoluzione dei problemi

## L'app si connette ma non mostra risultati

- La scheda portante del radar è alimentata? Radar Base è stato alimentato per primo? Apri Diagnostica → **Riavvia il radar**.
- Diagnostica → **Chiedi la versione al radar**: se risponde, i fili dei risultati (RX0/TX0) vanno bene.

## Il caricamento di uno scenario fallisce

- Diagnostica → **Controlla il collegamento di avvio**. Nessuna risposta di solito significa un filo allentato su RX1, TX1, RST, P12 o P19.
- Reinserisci i fili; i jumper perdono contatto facilmente. Tienili sotto i 15 cm.

## L'autotest dei dati grezzi mostra blocchi danneggiati o mancanti

- Controlla i fili SPI CLK e MOSI (MOSI, non MISO) e metti un filo di massa accanto.
- Abbassa la velocità SPI: console `cfg spi 12.5`.

## Tutti i blocchi sono errati e contengono solo 00 o FF

Il filo dei dati è sul pin MISO della scheda portante. Spostalo su **MOSI**.

## Il browser non elenca la porta USB

- Usa la porta contrassegnata **USB** sul controller, non UART.
- Chiudi qualsiasi terminale seriale: solo un programma alla volta può usare la porta.
- Usa Chrome o Edge su un computer desktop.

## http://kymradar.local non si apre

Alcuni telefoni Android non risolvono i nomi `.local`. Usa l'indirizzo IP mostrato in Impostazioni dispositivo → Wi-Fi di casa.

## Frequenza respiratoria o cardiaca resta vuota

Siediti o sdraiati fermo a 0,5-1,5 m davanti al radar, con il petto rivolto verso di esso, e attendi circa 20 secondi.
Queste misure sono solo per il benessere, non per uso medico.

## Viene mostrata una persona ma l'area non si illumina (o viceversa)

Aree e tracciamento sono uscite distinte del radar. Una persona molto ferma può uscire dal tracciamento mentre l'area
resta occupata. L'app mostra il conteggio più alto dell'ultimo momento per mantenere stabile il numero.

Volumi, moduli già programmati o firmware personalizzato: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
