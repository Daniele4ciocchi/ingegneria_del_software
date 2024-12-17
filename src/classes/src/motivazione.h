#ifndef MOTIVAZIONE_H
#define MOTIVAZIONE_H

/* System libraries */
#include <string.h>
#include <stdexcept>

/* Local libraries */
#include "../../shared/standard.h"
#include "../../service/redis/redisConnection.h"

/* Classes */
class Motivazione {
    public:
        char* id = NULL;
        char* motivo = NULL;
    
        Motivazione(char* id, char* motivo);
        ~Motivazione();
    
        static Motivazione* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif