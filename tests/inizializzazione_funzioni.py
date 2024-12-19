from generatori_parametri.generatore_nomi import NomePersona
from generatori_parametri.generatore_cognomi import CognomePersona
from generatori_parametri.generatore_cf import CF
from generatori_parametri.generatore_date import DataNascita, Data, DataInizio, DataFine
from generatori_parametri.generatore_indirizzi import Indirizzo
from generatori_parametri.generatore_stringhe import Stringa
from generatori_parametri.generatore_numeri_telefono import Telefono
from generatori_parametri.generatore_specializzazioni import Specializzazione
from generatori_parametri.generatore_id import IdPaziente, IdPazienteVisitato, IdMedico, IdAmministrativo, IdPrenotazionePendente, IdPrenotazioneAccettataConclusa, IdPrenotazioneAccettataNonConclusa, IdMotivazione
from generatori_parametri.generatore_voti import Voto

funzioni = {
            "registrazione" :  [[("cf", CF)],
                                [("nome", NomePersona)],
                                [("cognome", CognomePersona)],
                                [("nascita", DataNascita)],
                                [("indirizzo", Indirizzo)],
                                [("email", Stringa)],
                                [("telefono", Telefono)]],

            "nr_ricerca_medico" : [[("specializzazione", Specializzazione)]],


            "cronologia_prenotazioni" : [[("paziente_id", IdPaziente)]],

            "effettua_prenotazione" :  [[("paziente_id", IdPaziente)],
                                        [("medico_id", IdMedico)],
                                        [("amministrativo_id", IdAmministrativo)],
                                        [("specializzazione_nome", Specializzazione)],
                                        [("giornoorariopren", Data)]], 

            "lascia_feedback" :    [[("paziente_id", IdPazienteVisitato)],
                                    [("prenotazione_accettata_id", IdPrenotazioneAccettataConclusa)],
                                    [("votosodd", Voto)],
                                    [("votopunt", Voto)]],
            
            "ricerca_medico" : [[("specializzazione", Specializzazione)]],


            "cronologia_prestazioni" : [[("medico_id", IdMedico)]],

            "termina_prestazione" : [[("prenotazione_id", IdPrenotazioneAccettataNonConclusa)]],

            "statistiche" : [[("medico_id", IdMedico)]],


            "accetta_richpren" : [[("richiesta_id", IdPrenotazionePendente)]],

            "rifiuta_richpren" : [[("richiesta_id", IdPrenotazionePendente)],
                                  [("motivazione_id", IdMotivazione)]],

            "inserisci_medico" : [[("cf", CF)],
                                  [("nome", NomePersona)],
                                  [("cognome", CognomePersona)],
                                  [("nascita", DataNascita)]],

            "aggiungi_indisponibilita_medico" : [[("medico_id", IdMedico)],
                                                 [("inizioind", DataInizio)],
                                                 [("fineind", DataFine)]],

            "cronologia_richpren" : [[("amministrativo_id", IdAmministrativo)]],

            "aggiungi_specializzazione_medico" : [[("medico_id", IdMedico)],
                                                  [("specializzazione_nome", Specializzazione)]],
            }


apis = {
        "amministrativo" : ["accetta_richpren", "rifiuta_richpren", "inserisci_medico", "aggiungi_indisponibilita_medico", "aggiungi_specializzazione_medico", "cronologia_richpren"],
        "medico" : ["cronologia_prestazioni", "statistiche", "termina_prestazione"],
        "paziente" : ["ricerca_medico", "cronologia_prenotazioni", "effettua_prenotazione", "lascia_feedback"],
        "paziente_non_registrato" : ["registrazione", "nr_ricerca_medico"]
       }

porte = {"amministrativo" : 42070, "medico" : 42069, "paziente" : 42071, "paziente_non_registrato" : 42072}