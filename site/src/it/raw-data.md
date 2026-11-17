---
title: Dati grezzi e i tuoi algoritmi
nav: Dati grezzi
group: Develop
---
# Dati grezzi e i tuoi algoritmi

Radar Base può trasmettere via USB i dati grezzi del radar (ogni chirp, entrambi i canali di ricezione). Il firmware aperto include questo flusso e le sue impostazioni; lo strumento per PC per visualizzare ed elaborare i dati grezzi non è ancora pubblicato.

## Cosa offre il firmware aperto

| Funzione | Dove |
|---|---|
| Impostazioni dei dati grezzi: chirp, guadagno, filtri, antenne | App → Dati grezzi e impostazioni del radar, o console `cfg <key> <value>` |
| Autotest del collegamento dati: ogni blocco viene controllato | App → Dati grezzi e impostazioni del radar → **Avvia test** |
| Risultati in JSON per il tuo codice | [Risultati JSON e API](json-api) |

## Dove eseguire un algoritmo

Fai il prototipo sul PC. Poi verifica se l'hardware FFT e CFAR del radar può svolgere il compito; se sì, deve girare sul
radar e ne escono solo i risultati. Altrimenti controlla l'ESP32, e tienilo solo sul PC se nessuno dei due basta.

## Controlla il collegamento dati

App → Dati grezzi e impostazioni del radar → **Avvia test**. Esegue l'app dei dati grezzi per qualche secondo e controlla
ogni blocco: i blocchi danneggiati e quelli mancanti devono essere entrambi 0.

Volumi, moduli già programmati o firmware personalizzato: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
