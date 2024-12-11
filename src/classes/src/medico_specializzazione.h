#ifndef MEDICO_SPECIALIZZAZIONE_H
#define MEDICO_SPECIALIZZAZIONE_H

/* System libraries */
#include <string.h>
#include <stdexcept>

/* Local libraries */
#include "../../utils/src/const.h"
#include "../../lib/con2redis/src/con2redis.h"

/* Classes */
class MedicoSpecializzazione {
public:
    char* medico_id = NULL;
    char* specializzazione_nome = NULL;

    MedicoSpecializzazione(char* medico_id, char* specializzazione_nome);
    ~MedicoSpecializzazione();

    static MedicoSpecializzazione* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif