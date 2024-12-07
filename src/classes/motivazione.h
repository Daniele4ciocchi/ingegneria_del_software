#ifndef MOTIVAZIONE_H
#define MOTIVAZIONE_H

/* System libraries */
#include <string.h>
#include <stdexcept>

/* Local libraries */
#include "../../utils/src/const.h"
#include "../../lib/con2redis/src/con2redis.h"

/* Classes */
class Motivazione {
public:
    int id;
    char* motivo = NULL;

    Motivazione(int id, const char* motivo);
    ~Motivazione();

    static Motivazione* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif