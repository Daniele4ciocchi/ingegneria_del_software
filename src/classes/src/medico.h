#ifndef MEDICO_H
#define MEDICO_H

/* System libraries */
#include <string.h>
#include <stdexcept>

/* Local libraries */
#include "../../shared/standard.h"
#include "../../service/redis/redisConnection.h"

/* Classes */
class Medico {
    public:
        char* id = NULL;
        char* cf = NULL;

        Medico(char* id, char* cf);
        ~Medico();

        static Medico* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif