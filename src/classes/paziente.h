#ifndef paziente_h
#define paziente_h

#include <string.h>
#include <stdexcept>

// librerie locali

#include "../service/redis/redisConnection.h"
#include "../shared/standard.h"

// class

class Paziente{

    public:
        char* cf   = NULL;
        char* nome = NULL;
        char* cognome = NULL;
        char* nascita = NULL;
        char* indirizzo = NULL;
        char* email = NULL;
        char* telefono = NULL;

        Paziente(char* cf_paziente, char* nome_paziente, char* cognome_paziente, char* nascita_paziente, char* indirizzo_paziente, char* email_paziente, char* telefono_paziente);

        ~Paziente();

        static Paziente* from_stream(redisReply* reply, int stream_num, int msg_num);

};

#endif