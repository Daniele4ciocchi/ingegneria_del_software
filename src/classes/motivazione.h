#ifndef MOTIVAZIONE_H
#define MOTIVAZIONE_H

#include <string.h>
#include <stdexcept>

// class

class Motivazione {
public:
    int id;
    char* motivo;

    Motivazione(int id, const char* motivo);
    ~Motivazione();

    static Motivazione* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif // MOTIVAZIONE_H