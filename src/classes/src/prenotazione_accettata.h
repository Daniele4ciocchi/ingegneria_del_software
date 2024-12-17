#ifndef PRENOTAZIONE_ACCETTATA_H
#define PRENOTAZIONE_ACCETTATA_H

/* System libraries */
#include <string.h>
#include <stdexcept>

/* Local libraries */
#include "../../shared/standard.h"
#include "../../service/redis/redisConnection.h"

/* Classes */
class PrenotazioneAccettata {
public:
    char* richiesta_id = NULL;
    char* iaccet = NULL;
    char* prestazioneavvenuta = NULL;

    PrenotazioneAccettata(char* richiesta_id, char* iaccet, char* prestazioneavvenuta);
    ~PrenotazioneAccettata();

    static PrenotazioneAccettata* from_stream(redisReply* reply, int stream_num, int msg_num);
    std::string to_insert_query() const;
};

#endif