---
title: Per iniziare
nav: Per iniziare
group: Start
---
# Per iniziare

Questa guida ti porta dalla scatola ai risultati radar in tempo reale in circa 15 minuti.

## Cosa serve

- Controller Radar Base (scheda ESP32-S3) e il modulo radar a 60 GHz sulla sua scheda portante
- 10 jumper (femmina-femmina) - inclusi nel kit
- Due cavi USB: uno per Radar Base, uno per alimentare la scheda portante del radar
- Un computer con Chrome o Edge (Windows, macOS, Linux o ChromeOS)

## 1. Prepara la scheda portante del radar

Rimuovi tutti i ponticelli dal connettore seriale della scheda (J9) e dal connettore di boot P19 / GND / P12.
Altrimenti il chip USB-seriale della scheda entra in conflitto con Radar Base sugli stessi fili.

## 2. Collega i fili

Spegni prima tutto. Collega **prima GND**, poi il resto. Le etichette sono quelle stampate sulla scheda portante del radar.

[[wiring]]

| Pin di Radar Base | Etichetta sulla scheda del radar | Funzione |
|---|---|---|
| GND | GND | Massa comune (da collegare per prima) |
| GPIO17 | RX0 | Comandi all'app radar |
| GPIO18 | TX0 | Risultati dall'app radar |
| GPIO15 | RX1 | Caricamento delle app nel radar |
| GPIO16 | TX1 | Risposte durante il caricamento |
| GPIO4 | RST | Reset del radar |
| GPIO5 | P12 | Selezione del boot (anche selezione dei dati) |
| GPIO6 | P19 | Selezione del boot |
| GPIO12 | SPI CLK | Clock dei dati grezzi (solo per l'uso dei dati grezzi) |
| GPIO11 | SPI MOSI | Dati grezzi (non usare MISO) |

Il radar è alimentato dalla propria scheda portante (assorbe fino a circa 600 mA). Non alimentarlo mai dal pin 3V3 della
scheda del controller.

## 3. Accendi nell'ordine giusto

1. Collega al computer la porta del controller contrassegnata **USB**. (La porta contrassegnata UART serve solo ad
   aggiornare il firmware del controller stesso.)
2. Poi alimenta la scheda portante del radar.

Radar Base imposta i pin di boot del radar non appena si avvia, quindi il radar deve accendersi per secondo. Se li hai
alimentati in ordine inverso, apri Diagnostica e premi **Riavvia il radar**.

## 4. Guardalo in azione

1. Apri [l'app](app/) in Chrome o Edge e premi **Collega USB**; scegli la porta chiamata "USB JTAG/serial".
2. Scegli **Esegui uno scenario** → **Presenza 3D e tracciamento**.
3. Cammina davanti al radar. La tua posizione compare come un punto blu.

Poi: [Wi-Fi di casa](wifi) per l'uso quotidiano, oppure [Dati grezzi](raw-data) per creare i tuoi algoritmi.

Volumi, moduli già programmati o firmware personalizzato: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
