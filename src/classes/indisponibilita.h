#ifndef INDISPONIBILITA_H
#define INDISPONIBILITA_H

/* System libraries */
#include <string.h>
#include <stdexcept>
#include <ctime>

/* Local libraries */
#include "../../utils/src/const.h"
#include "../../lib/con2redis/src/con2redis.h"

/* Classes */
class Indisponibilita {
public:
    int medico_id;
    std::tm inizio;
    std::tm fine;

    Indisponibilita(int medico_id, const std::tm& inizio, const std::tm& fine);
    ~Indisponibilita();

    static Indisponibilita* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif