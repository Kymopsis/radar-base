---
title: Home Assistant
nav: Home Assistant
group: Smart home
---
# Home Assistant

Radar Base compare da solo in Home Assistant tramite la scoperta MQTT. Usi il tuo broker MQTT, di solito
il componente aggiuntivo **Mosquitto broker** di Home Assistant. Kymopsis non gestisce alcun broker né alcun cloud; nulla esce dalla tua rete.

## Cosa ti serve

- Home Assistant con il componente aggiuntivo **Mosquitto broker** e l'integrazione **MQTT**, oppure un altro broker MQTT che gestisci tu.
  Annota indirizzo del broker, porta (di solito 1883), utente e password.
- Una rete Wi-Fi a **2.4 GHz**, oppure una rete dual-band in cui 2.4 GHz e 5 GHz hanno lo stesso nome. L'ESP32-S3 non ha
  radio a 5 GHz.
- Firmware di Radar Base 0.9.1 o successivo ([installalo dal browser](app)).

## Configurazione

1. Collegati al tuo Wi-Fi: apri l'app di Radar Base (via USB o tramite l'hotspot della scheda), Impostazioni dispositivo, Wi-Fi.
2. In Impostazioni dispositivo, Smart home: inserisci indirizzo del broker, porta, utente e password, poi attiva MQTT.
3. In Home Assistant apri Impostazioni → Dispositivi e servizi → MQTT. **Kymopsis Radar Base** è lì con le sue entità.
   Non c'è altro da configurare.

## Entità

| Entità | Tipo | Unità / valori | Dall'app radar |
|---|---|---|---|
| Occupazione | Sensore binario | on / off | Presenza 3D |
| Numero di persone | Sensore | numero | Presenza 3D |
| Persona più vicina | Sensore | m | Presenza 3D |
| Area 1, Area 2, Area 3, Area 4 | Sensore binario | on / off | Presenza 3D |
| Caduta rilevata | Sensore binario | on / off | Rilevamento cadute |
| Frequenza respiratoria | Sensore | /min | Respirazione e battito cardiaco |
| Frequenza cardiaca | Sensore | /min | Respirazione e battito cardiaco |
| App radar | Sensore | nome dell'app in esecuzione | qualsiasi |
| Montaggio | Selezione | soffitto / parete | Presenza 3D |
| Riavvia radar | Pulsante | | qualsiasi |

I valori che l'app radar in esecuzione non segnala restano vuoti o sconosciuti. Frequenza respiratoria e cardiaca sono solo per il benessere, non per uso medico.

## Online e offline

La scheda pubblica un topic di disponibilità. Quando disattivi MQTT sulla scheda, il dispositivo diventa non disponibile
subito. Se la scheda perde alimentazione o Wi-Fi, il broker la segna come non disponibile dopo circa 45 secondi; torna da sola quando la scheda si riconnette.

## Risoluzione dei problemi

| Problema | Cosa fare |
|---|---|
| Il dispositivo non compare | Controlla la pagina Smart home della scheda: deve indicare connesso. Controlla utente e password nel broker. |
| La scheda non si collega al Wi-Fi | Usa una rete a 2.4 GHz o dual-band con un nome in comune; le reti solo a 5 GHz non funzionano. |
| `kymradar.local` non si apre | Una VPN o un proxy su quel computer può prendere il controllo dei nomi `.local`; disattivalo, lascia passare `.local` direttamente oppure apri l'indirizzo IP della scheda. |
| Le distanze appaiono in piedi | Home Assistant usa il tuo sistema di unità: Impostazioni → Sistema → Generali → Sistema di unità: Metrico. |

Volumi, moduli già programmati o firmware personalizzato: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
