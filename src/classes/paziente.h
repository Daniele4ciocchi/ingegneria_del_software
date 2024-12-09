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
    char* id = NULL;
    char* cf = NULL;
    char* indirizzo = NULL;
    char* email = NULL;
    char* telefono = NULL;
    

    Paziente(char* id, char* cf, char* indirizzo, char* email, char* telefono);
    ~Paziente();

    static Paziente* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif