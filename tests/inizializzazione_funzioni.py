from generatori_parametri.generatore_nomi import NomePersona
from generatori_parametri.generatore_cognomi import CognomePersona
from generatori_parametri.generatore_cf import CF
from generatori_parametri.generatore_date import DataNascita
from generatori_parametri.generatore_indirizzi import Indirizzo
from generatori_parametri.generatore_stringhe import Stringa
from generatori_parametri.generatore_numeri_telefono import Telefono
from generatori_parametri.generatore_specializzazioni import Specializzazione
#from generatori_parametri.generatore_id import IdPaziente, IdMedico, IdAmministrativo, IdPrenotazione_accettata

requests = {
 #funzioni paziente_non_registrato
 "registrazione" : [[("cf", CF)],
                    [("nome", NomePersona)],
                    [("cognome", CognomePersona)],
                    [("nascita", DataNascita)],
                    [("indirizzo", Indirizzo)],
                    [("email", Stringa)],
                    [("telefono", Telefono)]],

"ricerca_non_registrata" : [[("specializzazione", Specializzazione)]],

#funzioni paziente
#"cronologia_prenotazioni" : [[("paziente_id", IdPaziente)]],

#"effettua_prenotazione" : [[("paziente_id", IdPaziente)],
 #                          [("medico_id", IdMedico)],
  #                         [("amministrativo_id", IdAmministrativo)],
   #                        [("specializzazione", Specializzazione)] #mi puzza un po' 
    #                       ], 

"lascia_feedback" : [],
 
"ricerca_medico" : [[("specializzazione", Specializzazione)]],
}


apis = {"amministrativo" : ["accetta_richpren", "rifiuta_richpren", "aggiungi_indisponibilita_medico", "aggiungi_specializzazione_medico", "cronologia_richpren", "elimina_medico", "elimina_paziente"],
       "medico" : ["cronologia_prestazioni", "prestazioni_future", "statistiche", "termina_prestazione"],
       "paziente" : ["ricerca_medico", "cronologia_prenotazioni", "effettua_prenotazione", "lascia_feedback"],
       "paziente_non_registrato" : ["registrazione", "ricerca_non_registrata"]}

apis = {"paziente" : ["ricerca_medico"],
        "paziente_non_registrato" : ["registrazione"]
        }

ports = {"amministrativo" : 42070, "medico" : 42069, "paziente" : 42071, "paziente_non_registrato" : 42072}