#ifndef RICHIESTA_PRENOTAZIONE_H
#define RICHIESTA_PRENOTAZIONE_H

/* System libraries */
#include <string.h>
#include <stdexcept>
#include <chrono>
#include <ctime>

/* Local libraries */
#include "../../utils/src/const.h"
#include "../../lib/con2redis/src/con2redis.h"

/* Classes */
class RichiestaPrenotazione {
public:
    char* paziente_id = NULL;
    char* medico_id = NULL;
    char* amministrativo_id = NULL;
    char* specializzazione_nome = NULL;
    char* irich = NULL;
    char* giornoorariopren = NULL;

    RichiestaPrenotazione( char* paziente_id, char* medico_id, char* amministrativo_id, char* specializzazione_nome, char* irich, char* giornoorariopren);
    ~RichiestaPrenotazione();

    static RichiestaPrenotazione* from_stream(redisReply* reply, int stream_num, int msg_num);
    std::string to_insert_query();
    std::string to_update_query();

};

#endif