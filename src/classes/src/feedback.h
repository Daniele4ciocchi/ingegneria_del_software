#ifndef FEEDBACK_H
#define FEEDBACK_H

/* System libraries */
#include <string.h>
#include <stdexcept>
#include <chrono>
#include <ctime>

/* Local libraries */
#include "../../shared/standard.h"
#include "../../service/redis/redisConnection.h"

/* Classes */
class Feedback {
    public:
        char* paziente_id = NULL;
        char* prenotazione_accettata_id = NULL;
        char* ifeed = NULL;
        char* votosodd = NULL;
        char* votopunt = NULL;

        Feedback(char* paziente_id, char* prenotazione_accettata_id, char* ifeed, char* votosodd, char* votopunt);
        ~Feedback();

        static Feedback* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif