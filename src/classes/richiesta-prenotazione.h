#ifndef richiesta_prenotazione_h
#define richiesta_prenotazione_h

#include <string.h>
#include <stdexcept>

// librerie locali

#include "../service/redis/redisConnection.h"
#include "../shared/standard.h"

// class

class RichiestaPrenotazione {

public:
    int id;
    int paziente_id;
    int medico_id;
    int amministrativo_id;

    char* specializzazione_nome = NULL;
    char* irich = NULL;
    char* giornoorariopren = NULL;

    RichiestaPrenotazione(
        int id_richiesta,
        int id_paziente,
        int id_medico,
        int id_amministrativo,
        char* specializzazione,
        char* irich_timestamp,
        char* giornoorariopren_timestamp
    );

    ~RichiestaPrenotazione();

    static RichiestaPrenotazione* from_stream(redisReply* reply, int stream_num, int msg_num);

};

#endif
