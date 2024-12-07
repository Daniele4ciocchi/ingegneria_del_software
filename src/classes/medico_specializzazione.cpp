#include "medico_specializzazione.h"

MedicoSpecializzazione::MedicoSpecializzazione(int medico_id, char* specializzazione_nome) {
    this->medico_id = medico_id;
    this->specializzazione_nome = (char*) malloc(sizeof(char) * 101);
    strcpy(this->specializzazione_nome, specializzazione_nome);
}

MedicoSpecializzazione::~MedicoSpecializzazione() {
    free(this->specializzazione_nome);
}

MedicoSpecializzazione* MedicoSpecializzazione::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    int medico_id;
    char specializzazione_nome[101];

    char read_fields = 0b00;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "medico_id")) {
            medico_id = atoi(value);
            read_fields |= 0b01;

        } else if (!strcmp(key, "specializzazione_nome")) {
            snprintf(specializzazione_nome, 101, "%s", value);
            read_fields |= 0b10;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b11) {
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new MedicoSpecializzazione(medico_id, specializzazione_nome);
}