---
title: Metti Radar Base sul Wi-Fi di casa
nav: Wi-Fi di casa
group: Connect
---
# Metti Radar Base sul Wi-Fi di casa

Quando Radar Base è sulla tua rete di casa, qualsiasi telefono, tablet o computer in casa può aprirlo su
**http://kymradar.local**. I tuoi dispositivi restano sul normale Wi-Fi.

## Configurazione

Scegli un modo:

- **Dall'app (USB o hotspot):** Impostazioni dispositivo → Wi-Fi di casa → scegli la tua rete → inserisci la password → Connetti.
- **Da un terminale seriale:** `wifi <network-name> <password>` (senza spazi in entrambi).

Radar Base richiede una rete a 2,4 GHz. La maggior parte dei router domestici offre 2,4 GHz e 5 GHz con lo stesso nome, e va bene.

## Aprilo

- Su computer, iPhone, iPad e Mac: **http://kymradar.local**
- Su alcuni telefoni Android i nomi `.local` non vengono risolti. Usa l'indirizzo IP mostrato in Impostazioni
  dispositivo → Wi-Fi di casa (per esempio http://192.168.1.57), oppure cerca il dispositivo "kymradar" nell'elenco del router.

## Cosa succede all'hotspot

L'hotspot di Radar Base si spegne da solo quando il tuo Wi-Fi di casa funziona e nessun telefono è collegato. Si riaccende
da solo se la rete di casa sparisce per 30 secondi - vedi [Hotspot di riserva](hotspot).

## Cambiare o rimuovere la rete

Impostazioni dispositivo → Wi-Fi di casa → **Dimentica rete**, oppure sulla console `wifi forget`. L'hotspot si riaccende.

## Privacy

Radar Base parla solo con i dispositivi della tua rete. Non ha alcuna connessione cloud e non invia dati da nessuna parte.

Volumi, moduli già programmati o firmware personalizzato: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
