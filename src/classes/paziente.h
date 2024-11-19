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
        char* nome = NULL;
        char* mail = NULL;

        Customer(char* nome_paziente, char* email_paziente);

        ~Customer();

        static Customer* from_stream(redisReply* reply, int stream_num, int msg_num);

};

#endif