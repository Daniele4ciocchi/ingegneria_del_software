#ifndef PAZIENTE_H
#define PAZIENTE_H

/* System libraries */
#include <string.h>
#include <stdexcept>

/* Local libraries */
#include "../../utils/src/const.h"
#include "../../lib/con2redis/src/con2redis.h"

/* Classes */
class Paziente {
public:
    char* cf = NULL;
    char* email = NULL;
    char* telefono = NULL;

    //indirizzo
    char* via = NULL;
    char* numero_civico = NULL;
    char* cap = NULL;
    char* citta = NULL;
    char* provincia = NULL;
    

    Paziente( char* cf, char* email, char* telefono, char* via, char* numero_civico, char* cap, char* citta, char* provincia);
    ~Paziente();

    static Paziente* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif