---
title: Usalo via USB nel browser
nav: USB nel browser
group: Connect
---
# Usalo via USB nel browser

L'app di Radar Base gira in Chrome o Edge sul tuo computer e parla con la scheda tramite il cavo USB (Web Serial).
Niente da installare, nessun Wi-Fi coinvolto.

## Passaggi

1. Collega la porta **USB** del controller al computer.
2. Apri **[radar.kymopsis.com/app](app/)** in Chrome o Edge.
3. Premi **Collega USB** nella barra in alto e scegli "USB JTAG/serial debug unit".
4. Lo stato diventa verde. Scegli un'attività nella pagina iniziale.

Il browser chiede una volta per sito il permesso di usare la porta; nient'altro del tuo computer viene condiviso.

## Cosa funziona via USB

| Funzione | Via USB |
|---|---|
| Esegui uno scenario, viste in tempo reale, impostazioni dello scenario | Sì |
| Impostazioni dei dati grezzi e autotest del collegamento dati | Sì |
| Scrivere il firmware nel radar (kit di sviluppo) | Sì |
| Configurare il Wi-Fi di casa | Sì (nomi di rete e password senza spazi) |
| Caricare o eliminare file firmware | No - per questo usa il Wi-Fi |

## Un solo programma alla volta

Una porta USB può essere aperta in un solo programma. Chiudi qualsiasi terminale seriale prima di
collegare il browser, e viceversa.

## Browser

Web Serial è disponibile in Chrome, Edge e altri browser basati su Chromium su desktop. Safari e Firefox non lo
supportano; con quelli, o sui telefoni, usa il [Wi-Fi di casa](wifi).

Volumi, moduli già programmati o firmware personalizzato: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
