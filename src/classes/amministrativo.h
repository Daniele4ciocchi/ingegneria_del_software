#ifndef amministrativo_h
#define amministrativo_h

#include <string.h>
#include <stdexcept>

// librerie locali

#include "../service/redis/redisConnection.h"
#include "../shared/standard.h"

// class

class Amministrativo{

public:
    char* cf   = NULL;
    char* nome = NULL;
    char* cognome = NULL;
    char* nascita = NULL;

    Amministrativo(char* cf_amministrativo, char* nome_amministrativo, char* cognome_amministrativo, char* nascita_amministrativo);

    ~Amministrativo();

    static Amministrativo* from_stream(redisReply* reply, int stream_num, int msg_num);

};

#endif