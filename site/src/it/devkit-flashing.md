---
title: Modulo di scrittura del radar (kit di sviluppo)
nav: Scrittura del kit di sviluppo
group: Develop
---
# Modulo di scrittura del radar (kit di sviluppo)

Il firmware di Radar Base esiste in due edizioni, compilate dallo stesso codice sorgente:

| Edizione | Cosa sa fare |
|---|---|
| Aperta | Tutto ciò che c'è su questo sito: esegue ogni app radar dalla memoria del radar, JSON, app web, Wi-Fi, smart home |
| Kit di sviluppo | Aperta + un modulo in più che scrive nella flash del radar |

## Cosa fa il modulo

Un'app scritta nella flash del radar parte da sola all'accensione, senza che Radar Base debba caricarla prima. Il modulo
esegue un backup della flash attuale del radar prima di ogni scrittura; se il backup fallisce, non viene scritto nulla.
I backup si possono ripristinare in qualsiasi momento dall'app.

## Come viene consegnato il firmware del kit di sviluppo

Il firmware del kit di sviluppo è precaricato sulla scheda del kit. Gli aggiornamenti vengono comunicati agli acquirenti via e-mail; questo firmware non è un download pubblico. Il firmware aperto è pubblicato come codice sorgente: [github.com/Kymopsis/radar-base](https://github.com/Kymopsis/radar-base).

## Scrittura in serie

Per molti moduli di fila: scegli il firmware una sola volta e collega i moduli uno dopo l'altro. Ogni modulo viene rilevato, salvato se vuoi, scritto, verificato, avviato dalla sua flash e interrogato sulla versione. La pagina e il LED della scheda mostrano PASS o FAIL, un contatore somma e ogni modulo ottiene una riga in un registro CSV che puoi scaricare.

## Cosa non fa

- Non si connette a internet.
- Non raccoglie, non memorizza e non invia alcun dato.
- Nient'altro nel firmware dipende da lui.

È chiuso perché il piccolo programma ausiliario che carica nel radar per scrivere non possiamo pubblicarlo noi. Sulle
unità del kit di sviluppo, quel programma è memorizzato cifrato con una chiave unica per ogni unità.

## Senza il modulo

Ogni app radar gira anche dalla memoria del radar con il firmware aperto (Esegui uno scenario). Perdi solo la comodità
di «parte da sola senza Radar Base». Il firmware aperto, l'app e il formato JSON sono aperti per scelta progettuale; l'app e il formato JSON sono documentati su questo sito.

Volumi, moduli già programmati o firmware personalizzato: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
