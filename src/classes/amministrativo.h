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
        char* nome = NULL;
        char* mail = NULL;

        Amministrativo(char* nome_amministrativo, char* email_amministrativo);

        ~Amministrativo();

        static Amministrativo* from_stream(redisReply* reply, int stream_num, int msg_num);

};

#endif