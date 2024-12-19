#ifndef SPECIALIZZAZIONE_MEDICO_H
#define SPECIALIZZAZIONE_MEDICO_H

/* System libraries */
#include <string.h>
#include <stdexcept>

/* Local libraries */
#include "../../utils/src/const.h"
#include "../../lib/con2redis/src/con2redis.h"

/* Classes */
class SpecializzazioneMedico {
public:
    char* medico_id = NULL;
    char* specializzazione_nome = NULL;

    SpecializzazioneMedico(char* medico_id, char* specializzazione_nome);
    ~SpecializzazioneMedico();

    static SpecializzazioneMedico* from_stream(redisReply* reply, int stream_num, int msg_num);
    std::string to_insert_query() const;
};

#endif