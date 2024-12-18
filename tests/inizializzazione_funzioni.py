from generatori_parametri.generatore_nomi import NomePersona
from generatori_parametri.generatore_cognomi import CognomePersona
from generatori_parametri.generatore_cf import CF
from generatori_parametri.generatore_date import DataNascita, Data
from generatori_parametri.generatore_indirizzi import Indirizzo
from generatori_parametri.generatore_stringhe import Stringa
from generatori_parametri.generatore_numeri_telefono import Telefono
from generatori_parametri.generatore_specializzazioni import Specializzazione
from generatori_parametri.generatore_id import IdPaziente, IdPazienteVisitato, IdMedico, IdAmministrativo, IdPrenotazionePendente, IdPrenotazioneAccettataConclusa, IdPrenotazioneAccettataNonConclusa, IdMotivazione
from generatori_parametri.generatore_voti import Voto

requests = {
 #funzioni paziente_non_registrato
 "registrazione" : [[("cf", CF)],
                    [("nome", NomePersona)],
                    [("cognome", CognomePersona)],
                    [("nascita", DataNascita)],
                    [("indirizzo", Indirizzo)],
                    [("email", Stringa)],
                    [("telefono", Telefono)]],

"nr_ricerca_medico" : [[("specializzazione", Specializzazione)]],

#funzioni paziente
"cronologia_prenotazioni" : [[("paziente_id", IdPaziente)]],

"effettua_prenotazione" : [[("paziente_id", IdPaziente)],
                           [("medico_id", IdMedico)],
                           [("amministrativo_id", IdAmministrativo)],
                           [("specializzazione_nome", Specializzazione)],
                           [("giornoorariopren", Data)]
                           ], 

"lascia_feedback" : [[("paziente_id", IdPazienteVisitato)],
                     [("prenotazione_accettata_id", IdPrenotazioneAccettataConclusa)],
                     [("votosodd", Voto)],
                     [("votopunt", Voto)]],
 
"ricerca_medico" : [[("specializzazione", Specializzazione)]],

#funzioni medico
"cronologia_prestazioni" : [[("medico_id", IdMedico)]],

"termina_prestazione" : [[("prenotazione_id", IdPrenotazioneAccettataNonConclusa)]],

"statistiche" : [[("medico_id", IdMedico)]],

#funzioni amministrativo
"accetta_richpren" : [[("richiesta_id", IdPrenotazionePendente)]],

"rifiuta_richpren" : [[("richiesta_id", IdPrenotazionePendente)],
                      [("motivazione_id", IdMotivazione)]]
}


apis = {"amministrativo" : ["accetta_richpren", "rifiuta_richpren", "aggiungi_indisponibilita_medico", "aggiungi_specializzazione_medico", "cronologia_richpren", "elimina_medico", "elimina_paziente"],
       "medico" : ["cronologia_prestazioni", "statistiche", "termina_prestazione"],
       "paziente" : ["ricerca_medico", "cronologia_prenotazioni", "effettua_prenotazione", "lascia_feedback"],
       "paziente_non_registrato" : ["registrazione", "ricerca_non_registrata"]}

apis = {"amministrativo" : ["accetta_richpren", "rifiuta_richpren"],
        "medico" : ["cronologia_prestazioni", "termina_prestazione","statistiche"],
        #"paziente" : ["ricerca_medico", "cronologia_prenotazioni", "effettua_prenotazione", "lascia_feedback"],
        #"paziente_non_registrato" : ["registrazione", "nr_ricerca_medico"]
        }

ports = {"amministrativo" : 42070, "medico" : 42069, "paziente" : 42071, "paziente_non_registrato" : 42072}