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
    char* richiesta_id = NULL;
    char* irif = NULL;
    char* motivazione_id = NULL;


    PrenotazioneRifiutata(char* richiesta_id, char* irif, char* motivazione_id);
    ~PrenotazioneRifiutata();

    static PrenotazioneRifiutata* from_stream(redisReply* reply, int stream_num, int msg_num);
    std::string to_insert_query() const;
};

#endif