#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <ctime>
#include <stdexcept>
#include <string.h>

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

#endif // FEEDBACK_H