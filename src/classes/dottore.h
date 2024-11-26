#ifndef dottore_h
#define dottore_h

#include <string.h>
#include <stdexcept>

// librerie locali

#include "../service/redis/redisConnection.h"
#include "../shared/standard.h"

// class

class Dottore{

    public:
        char* nome = NULL;
        char* mail = NULL;

        Dottore(char* nome_dottore, char* email_dottore);

        ~Dottore();

        static dottore* from_stream(redisReply* reply, int stream_num, int msg_num);

};

#endif