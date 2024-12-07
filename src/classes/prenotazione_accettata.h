#ifndef PRENOTAZIONE_ACCETTATA_H
#define PRENOTAZIONE_ACCETTATA_H

/* System libraries */
#include <string.h>
#include <stdexcept>

/* Local libraries */
#include "../../utils/src/const.h"
#include "../../lib/con2redis/src/con2redis.h"

/* Classes */
class PrenotazioneAccettata {
public:
    int id;
    char* data;
    char* ora;
    char* medico_cf;
    char* paziente_cf;

    PrenotazioneAccettata(int id, char* data, char* ora, char* medico_cf, char* paziente_cf);
    ~PrenotazioneAccettata();

    static PrenotazioneAccettata* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif