#ifndef AMMINISTRATIVO_H
#define AMMINISTRATIVO_H

/* System libraries */
#include <string.h>
#include <stdexcept>

/* Local libraries */
#include "../../shared/standard.h"
#include "../../service/redis/redisConnection.h"

/* Classes */
class Amministrativo {
public:
    char* id_amministrativo = NULL;
    char* cf_amministrativo = NULL;

    Amministrativo(char* id_amministrativo, char* cf_amministrativo);
    ~Amministrativo();

    static Amministrativo* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif