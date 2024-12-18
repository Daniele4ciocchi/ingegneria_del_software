#include "main.h"

int main(){
    int num_types = 7;
    std::string req_types[num_types] = {"accetta_richpren", "rifiuta_richpren", "aggiungi_indisponibilita_medico", "aggiungi_specializzazione_medico", "cronologia_richpren", "elimina_medico", "elimina_paziente"};

    Server server(SERVER_ID, 42070, REDIS_SERVER, REDIS_PORT, req_types, num_types);

    server.run();
}