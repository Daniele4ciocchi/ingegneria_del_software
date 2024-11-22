#ifndef richiesta-prenotazione_h
#define richiesta-prenotazione_h

#include <string.h>
#include <stdexcept>

// librerie locali

#include "../service/redis/redisConnection.h"
#include "../shared/standard.h"

// class

class RichiestaPrenotazione{

    public:
        char* nome_paziente = NULL;
        char* mail_paziente = NULL;
        char* tipo_visita = NULL;
        char* data_visita = NULL;
        char* data_prenotazione = NULL;
        char* nome_medico = NULL;
        char* mail_medico = NULL;

        RichiestaPrenotazione(
                                char* nome_paziente, 
                                char* mail_paziente, 
                                char* tipo_visita,
                                char* data_visita,
                                char* data_prenotazione,
                                char* nome_medico,
                                char* mail_medico
                                );

        ~RichiestaPrenotazione();

        static RichiestaPrenotazione* from_stream(redisReply* reply, int stream_num, int msg_num);

};

#endif