#ifndef INDISPONIBILITA_H
#define INDISPONIBILITA_H

#include <ctime>
#include <stdexcept>
#include <string.h>

class Indisponibilita {
public:
    int id;
    int medico_id;
    std::tm inizioind;
    std::tm fineind;

    Indisponibilita(int id, int medico_id, const std::tm& inizioind, const std::tm& fineind);
    ~Indisponibilita();

    static Indisponibilita* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif // INDISPONIBILITA_H