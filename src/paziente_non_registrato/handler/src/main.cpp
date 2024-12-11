#include "main.h"

int main(){
    int num_types = 2;
    std::string req_types[num_types] = {"registrazione", "ricerca_medico"};

    Server server(SERVER_ID, 42072, REDIS_SERVER, REDIS_PORT, req_types, num_types);

    server.run();
}