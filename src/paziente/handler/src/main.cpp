#include "main.h"

int main()
{
    int num_types = 4;
    std::string req_types[num_types] = {"cronologia_prenotazioni", "effettua_prenotazione", "lascia_feedback", "ricerca_medico"};

    Server server(SERVER_ID, 42071, REDIS_SERVER, REDIS_PORT, req_types, num_types);

    server.run();
}