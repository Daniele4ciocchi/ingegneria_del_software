#ifndef PAZIENTE_H
#define PAZIENTE_H

/* System libraries */
#include <string.h>
#include <stdexcept>

/* Local libraries */
#include "../../shared/standard.h"
#include "../../service/redis/redisConnection.h"

/* Classes */
class Paziente {
public:
    char* cf = NULL;
    char* indirizzo = NULL;
    char* email = NULL;
    char* telefono = NULL;
    

    Paziente( char* cf, char* indirizzo, char* email, char* telefono);
    ~Paziente();

    static Paziente* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif