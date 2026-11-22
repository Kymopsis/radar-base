---
title: Download
nav: Download
group: Develop
---
# Download

## Firmware

Il modo più semplice è il pulsante **Installa il firmware** nella [pagina iniziale](app) (Chrome o Edge su desktop, scheda via USB). Gli stessi file sono su GitHub: [release di Radar Base](https://github.com/Kymopsis/radar-base/releases).
Il firmware 0.9.2 e successivi includono le app radar (presenza 3D; frequenza respiratoria e cardiaca).

## Radar Studio per Windows

[Scarica Radar Studio 0.9.1 per Windows](https://github.com/Kymopsis/radar-base/releases/download/v0.9.1/KymopsisRadarStudio-0.9.1.exe)
(64 bit, circa 60 MB, senza installazione; SHA-256 nella
[release](https://github.com/Kymopsis/radar-base/releases/tag/v0.9.1)).

| Funziona con | Funzioni |
|---|---|
| Qualsiasi Radar Base | Connessione via USB, viste per scenario, impostazioni comuni, configurazione smart home (MQTT) |
| Kit di sviluppo Kymopsis Radar Base | Inoltre dati grezzi, range-Doppler, impostazioni di forma d'onda e filtri, i tuoi algoritmi, parametri vitali / HRV |

### «Windows ha protetto il PC»

Radar Studio non è ancora firmato digitalmente, quindi Windows SmartScreen può bloccarlo la prima volta. Scegli **Ulteriori informazioni**, poi **Esegui comunque**. Confronta lo SHA-256 del file con la pagina della release se vuoi essere sicuro che sia l'originale.

## Codice sorgente

Firmware aperto, app web, componente ESPHome e libreria Arduino: [github.com/Kymopsis/radar-base](https://github.com/Kymopsis/radar-base).

Volumi, moduli già programmati o firmware personalizzato: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
