from generatori_parametri.generatore_nomi import NomePersona
from generatori_parametri.generatore_cognomi import CognomePersona
from generatori_parametri.generatore_cf import CF
from generatori_parametri.generatore_data import DataNascita, Data

requests = {
 "registrazione" : [[("cf", CF)],
                    [("nome", NomePersona)],
                    [("cognome", CognomePersona)],
                    [("nascita", DataNascita)]]
 
}


apis = {"amministrativo" : ["accetta_richpren", "rifiuta_richpren", "aggiungi_indisponibilita_medico", "aggiungi_specializzazione_medico", "cronologia_richpren", "elimina_medico", "elimina_paziente"],
       "medico" : ["cronologia_prestazioni", "prestazioni_future", "statistiche", "termina_prestazione"],
       "paziente" : ["ricerca_medico", "cronologia_prenotazioni", "effettua_prenotazione", "lascia_feedback"],
       "paziente_non_registrato" : ["registrazione", "ricerca_medico"]}

apis = {"paziente_non_registrato" :  ["registrazione"]}

ports = {"amministrativo" : 42070, "medico" : 42069, "paziente" : 42071, "paziente_non_registrato" : 42072}