#include "main.h"

int main()
{
    int num_types = 3;
    std::string req_types[num_types] = {"cronologia_prestazioni", "statistiche", "termina_prestazione"};

    Server server(SERVER_ID, 42069, REDIS_SERVER, REDIS_PORT, req_types, num_types);

    server.run();
}