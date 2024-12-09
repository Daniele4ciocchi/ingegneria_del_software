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
        char* id_indisponibilita = NULL;
        char* medico_id;
        char* inizio;
        char* fine;


        Indisponibilita(char* id_indisponibilita, char* medico_id, char* inizio, char* fine);
        ~Indisponibilita();

        static Indisponibilita* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif