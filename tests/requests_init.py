from generatori_parametri.stringnc import StringNC
from generatori_parametri.date import BornDate, Date
from generatori_parametri.cf import CF

requests = {
 "registrazione" : [[("cf", CF)],
                    [("nome", StringNC)],
                    [("cognome", StringNC)],
                    [("nascita", BornDate)]],

 
}


apis = {"amministrativo" : ["accetta_richpren", "rifiuta_richpren", "aggiungi_indisponibilita_medico", "aggiungi_specializzazione_medico", "cronologia_richpren", "elimina_medico", "elimina_paziente"],
       "medico" : ["cronologia_prestazioni", "prestazioni_future", "statistiche", "termina_prestazione"],
       "paziente" : ["ricerca_medico", "cronologia_prenotazioni", "effettua_prenotazione", "lascia_feedback"],
       "paziente_non_registrato" : ["registrazione", "ricerca_medico"]}

ports = [("amministrativo", 42070), ("medico", 42069), ("paziente", 42071), ("paziente_non_registrato", 42072)]