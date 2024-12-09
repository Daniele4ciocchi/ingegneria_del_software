#ifndef RICHIESTA_PRENOTAZIONE_H
#define RICHIESTA_PRENOTAZIONE_H

/* System libraries */
#include <string.h>
#include <stdexcept>

/* Local libraries */
#include "../../utils/src/const.h"
#include "../../lib/con2redis/src/con2redis.h"

/* Classes */
class RichiestaPrenotazione {
public:
    char* id = NULL;
    char* paziente_id = NULL;
    char* medico_id = NULL;
    char* amministrativo_id = NULL;
    char* specializzazione_nome = NULL;
    char* giornoorariopren = NULL;

    RichiestaPrenotazione(char* id, char* paziente_id, char* medico_id, char* amministrativo_id, char* specializzazione_nome, char* giornoorariopren);
    ~RichiestaPrenotazione();

    static RichiestaPrenotazione* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif