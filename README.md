### Requisiti funzionali
Il nostro progetto consiste nella progettazione del backend di una piattaforma di gestione di appuntamenti medici alla quale possono essere fatte richieste di diverso tipo. 
Nel sistema vi sono quattro tipi di entità che possono effettuare diverse operazioni al suo interno, nello specifico abbiamo:  
#### Paziente non registrato 
- Registrazione
- Ricerca medico
#### Paziente
- Cronologia prenotazioni
- Effettua prenotazione
- Lascia feedback
- Ricerca medico
#### Medico
- Cronologia prestazioni 
- Statistiche
- Termina prestazione
#### Amministrativo
- Aggiungi indisponibilità medico
- Aggiungi specializzazione medico
- Cronologia richieste prenotazione
- Accetta richiesta prenotazione
- Rifiuta richiesta prenotazione

### Requisiti non funzionali

Inoltre il sistema consente di controllare tramite requisiti non funzionali i tempi di risposta nelle connessioni al database.

Di seguito i comandi per testare il sistema:

- Per installare i pacchetti necessari alla compilazione del progetto
```sh
sh install.sh
```

- Per compilare il progetto 
```sh
sh compile.sh
```

- Per eseguire il progetto 
```sh
sh run.sh
```


Per eseguire il test del progetto 
```sh 
cd /tests
python3 tester.py
```


#### Progetto realizzato da
- Sepe Gabriele
- Quattrociocchi Daniele
- Mastromattei Luca