#ifndef AMMINISTRATIVO_H
#define AMMINISTRATIVO_H

/* System libraries */
#include <string.h>
#include <stdexcept>

/* Local libraries */
#include "../../utils/src/const.h"
#include "../../lib/con2redis/src/con2redis.h"

/* Classes */
class Amministrativo {
public:
    char* cf = NULL;
    char* nome = NULL;
    char* cognome = NULL;
    char* nascita = NULL;

    Amministrativo(char* cf_amministrativo, char* nome_amministrativo, char* cognome_amministrativo, char* nascita_amministrativo);
    ~Amministrativo();

    static Amministrativo* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif