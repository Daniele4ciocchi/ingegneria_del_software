#include "medico_specializzazione.h"

MedicoSpecializzazione::MedicoSpecializzazione(char* medico_id, char* specializzazione_nome) {

    this->medico_id = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->specializzazione_nome = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(this->medico_id, medico_id);
    strcpy(this->specializzazione_nome, specializzazione_nome);

}

MedicoSpecializzazione::~MedicoSpecializzazione() {
    free(this->medico_id);
    free(this->specializzazione_nome);

}

MedicoSpecializzazione* MedicoSpecializzazione::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    char medico_id[PARAMETERS_LEN];
    char specializzazione_nome[PARAMETERS_LEN];

    char read_fields = 0b00;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "medico_id")) {
            strcpy(medico_id, value);
            read_fields |= 0b01;

        } else if (!strcmp(key, "specializzazione_nome")) {
            strcpy(specializzazione_nome, value);
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