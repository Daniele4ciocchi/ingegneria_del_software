#ifndef FEEDBACK_H
#define FEEDBACK_H

/* System libraries */
#include <string.h>
#include <stdexcept>
#include <ctime>

/* Local libraries */
#include "../../utils/src/const.h"
#include "../../lib/con2redis/src/con2redis.h"

/* Classes */
class Feedback {
public:
    int paziente_id;
    int prenotazione_accettata_id;
    std::tm ifeed;
    int votosodd;
    int votopunt;

    Feedback(int paziente_id, int prenotazione_accettata_id, const std::tm& ifeed, int votosodd, int votopunt);
    ~Feedback();

    static Feedback* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif