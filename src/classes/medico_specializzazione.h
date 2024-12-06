#ifndef MEDICO_SPECIALIZZAZIONE_H
#define MEDICO_SPECIALIZZAZIONE_H

#include <stdexcept>
#include <string.h>

class MedicoSpecializzazione {
public:
    int medico_id;
    char* specializzazione_nome;

    MedicoSpecializzazione(int medico_id, const char* specializzazione_nome);
    ~MedicoSpecializzazione();

    static MedicoSpecializzazione* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif // MEDICO_SPECIALIZZAZIONE_H