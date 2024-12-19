### Requisiti funzionali
Il nostro progetto prevede la progettazione di un sistema di gestione di una piattaforma ospedaliera a cui possono essere fatte diverse richieste 
Troviamo quattro tipi di entità che possono effettuare operazioni all'interno del sistema e per ognuna sono definite delle funzioni effettuabili come segue 
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

Inoltre il sistema consente di controllare tramite un requisito non funzionale i tempi di risposta nelle connessioni al db nell'arco di un minuto

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


#### Progetto creato da
- Sepe Gabriele
- Quattrociocchi Daniele
- Mastromattei Luca