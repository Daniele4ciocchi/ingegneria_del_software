#ifndef medico_h
#define medico_h

#include <string.h>
#include <stdexcept>

// librerie locali

#include "../service/redis/redisConnection.h"
#include "../shared/standard.h"

// class

class Medico {

public:
    char* cf   = NULL;
    char* nome = NULL;
    char* cognome = NULL;
    char* nascita = NULL;

    Medico(char* cf_medico, char* nome_medico, char* cognome_medico, char* nascita_medico);

    ~Medico();

    static Medico* from_stream(redisReply* reply, int stream_num, int msg_num);

};

#endif
