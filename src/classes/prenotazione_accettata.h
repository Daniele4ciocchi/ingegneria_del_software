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
    char* richiesta_id = NULL;
    char* iaccet = NULL;
    char* prestazioneavvenuta = NULL;

    PrenotazioneAccettata(char* richiesta_id, char* iaccet, char* prestazioneavvenuta);
    ~PrenotazioneAccettata();

    static PrenotazioneAccettata* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif