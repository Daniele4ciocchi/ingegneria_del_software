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
    int id;
    char* data;
    char* ora;
    char* medico_cf;
    char* paziente_cf;

    RichiestaPrenotazione(int id, const char* data, const char* ora, const char* medico_cf, const char* paziente_cf);
    ~RichiestaPrenotazione();

    static RichiestaPrenotazione* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif