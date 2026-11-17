---
title: Home Assistant
nav: Home Assistant
group: Smart home
---
# Home Assistant

> In arrivo. Questa pagina descrive come funzionerà l'integrazione; è in sviluppo adesso.

Radar Base comparirà da solo in Home Assistant tramite la scoperta MQTT. Usi il tuo broker MQTT; la scelta abituale è
il componente aggiuntivo **Mosquitto broker** di Home Assistant. Kymopsis non gestisce alcun broker né alcun cloud.

## Entità previste

| Entità | Tipo | Origine |
|---|---|---|
| Occupazione | Sensore binario | Presenza 3D |
| Numero di persone | Sensore | Presenza 3D |
| Area 1-4 occupata | Sensori binari | Presenza 3D |
| Caduta rilevata | Sensore binario ed evento | Rilevamento cadute |
| Frequenza respiratoria, frequenza cardiaca | Sensori (diagnostica) | Respirazione e battito cardiaco |
| Montaggio (soffitto / parete) | Selezione | Presenza 3D |

## Configurazione prevista

1. In Home Assistant installa il componente aggiuntivo Mosquitto broker e l'integrazione MQTT.
2. Nell'app di Radar Base: Impostazioni dispositivo → Smart home → inserisci indirizzo del broker, utente e password.
3. Radar Base compare in Impostazioni → Dispositivi.

Frequenza respiratoria e cardiaca sono solo per il benessere, non per uso medico.

Volumi, moduli già programmati o firmware personalizzato: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
