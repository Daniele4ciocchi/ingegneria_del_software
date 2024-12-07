#ifndef PRENOTAZIONE_RIFIUTATA_H
#define PRENOTAZIONE_RIFIUTATA_H

/* System libraries */
#include <string.h>
#include <stdexcept>

/* Local libraries */
#include "../../utils/src/const.h"
#include "../../lib/con2redis/src/con2redis.h"

/* Classes */
class PrenotazioneRifiutata {
public:
    int id;
    char* data;
    char* ora;
    char* medico_cf;
    char* paziente_cf;
    char* motivo;

    PrenotazioneRifiutata(int id, char* data, char* ora, char* medico_cf, char* paziente_cf, char* motivo);
    ~PrenotazioneRifiutata();

    static PrenotazioneRifiutata* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif