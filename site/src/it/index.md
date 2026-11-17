---
title: Kymopsis Radar Base
nav: Panoramica
---
# Radar Base

Radar Base collega un modulo radar mmWave a 60 GHz al tuo computer, al tuo telefono e alla tua smart home.
Il lavoro radar lo fa il radar sul proprio chip; Radar Base vi carica le app radar, trasforma i loro risultati in un
semplice formato JSON e li rende disponibili via USB e Wi-Fi.

[[cards]]
- **Esegui un'app radar già pronta** | Posizioni delle persone in 3D, rilevamento cadute, respirazione e battito cardiaco. Senza programmare. | getting-started
- **Usalo nel browser via USB** | Collega, apri questo sito in Chrome o Edge, fatto. Nessun Wi-Fi necessario. | usb
- **Mettilo sul Wi-Fi di casa** | Raggiungilo su http://kymradar.local da qualsiasi telefono o computer in casa. | wifi
- **Crea la tua integrazione** | Leggi i risultati in JSON via USB o WebSocket da qualsiasi programma. | json-api
[[/cards]]

## Hardware testato

| Hardware | Stato |
|---|---|
| Kit di sviluppo Kymopsis Radar Base | Testato. Funziona subito; profilo scheda incluso |
| Montaggio DIY Kymopsis: ESP32-S3 DevKit e il modulo radar a 60 GHz sulla sua scheda portante, cablati come in [Per iniziare](getting-started) | Testato. Funziona subito; profilo scheda incluso |
| Altre schede e moduli radar | Non testato. Controlla tu l'assegnazione dei pin e il profilo scheda; l'app mostrerà allora «Scheda sconosciuta, angoli non calibrati» |

## Edizioni aperta e kit di sviluppo

| Funzione | Firmware aperto | Firmware del kit di sviluppo |
|---|---|---|
| Eseguire app radar dalla memoria del radar, viste in tempo reale | Sì | Sì |
| JSON via USB e Wi-Fi, app web in 11 lingue | Sì | Sì |
| Wi-Fi di casa, hotspot di riserva, Home Assistant (MQTT) | Sì | Sì |
| Flusso di dati grezzi, impostazioni e autotest | Sì | Sì |
| Scrivere un'app nella flash del radar, con backup | - | Sì |
| Scrittura in serie: PASS / FAIL, contatore, registro CSV | - | Sì |
| Profilo scheda impostato in fabbrica | - | Sì |

## Come si incastra tutto

[[diagram]]

| Parte | Cosa fa |
|---|---|
| Modulo radar | Esegue le app radar: ogni app svolge l'intera catena del segnale sul radar e riporta i risultati |
| Radar Base (ESP32-S3) | Carica le app radar, traduce i risultati in JSON, trasmette i dati grezzi, serve l'app web, si unisce al tuo Wi-Fi |
| App web | Scegli un'attività, guarda i risultati in tempo reale, cambia le impostazioni. Stessa app via Wi-Fi e via USB, in 11 lingue |
| Il tuo codice | Legge i risultati JSON via USB o WebSocket |
| Smart home | Home Assistant tramite MQTT (in arrivo), ESPHome (più avanti) |

## Modi di collegarsi

| Modalità | Quando usarla | Guida |
|---|---|---|
| USB | Primi passi e sviluppo. Funziona ovunque, senza rete | [USB nel browser](usb) |
| Wi-Fi di casa | Uso quotidiano da telefoni e computer in casa | [Wi-Fi di casa](wifi) |
| Hotspot | Prima configurazione, o quando nessuna rete di casa è raggiungibile | [Hotspot di riserva](hotspot) |
| Home Assistant | Automazioni: luci, avvisi, dashboard | [Home Assistant](home-assistant) |

Radar Base non richiede mai un account cloud. I tuoi dati radar restano sui tuoi dispositivi.

Volumi, moduli già programmati o firmware personalizzato: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
