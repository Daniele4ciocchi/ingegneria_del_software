#ifndef MEDICO_H
#define MEDICO_H

/* System libraries */
#include <string.h>
#include <stdexcept>

/* Local libraries */
#include "../../utils/src/const.h"
#include "../../lib/con2redis/src/con2redis.h"

/* Classes */
class Medico {
public:
    int id;
    char* nome = NULL;
    char* cognome = NULL;
    char* specializzazione = NULL;

    Medico(int id, char* nome, char* cognome, char* specializzazione);
    ~Medico();

    static Medico* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif