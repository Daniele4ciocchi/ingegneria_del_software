#ifndef INDISPONIBILITA_H
#define INDISPONIBILITA_H

/* System libraries */
#include <string.h>
#include <stdexcept>
#include <ctime>

/* Local libraries */
#include "../../utils/src/const.h"
#include "../../lib/con2redis/src/con2redis.h"

/* Classes */
class Indisponibilita {
    public:
        char* medico_id = NULL;
        char* inizio = NULL;
        char* fine = NULL;


        Indisponibilita( char* medico_id, char* inizio, char* fine);
        ~Indisponibilita();

        static Indisponibilita* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif