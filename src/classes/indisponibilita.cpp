#include "indisponibilita.h"

Indisponibilita::Indisponibilita(char* id_indisponibilita, char* medico_id, char* inizio, char* fine) {

    this->id_indisponibilita = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->medico_id = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->inizio = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->fine = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    
    strcpy(this->id_indisponibilita, id_indisponibilita);
    strcpy(this->medico_id, medico_id);
    strcpy(this->inizio, inizio);
    strcpy(this->fine, fine);

}

Indisponibilita::~Indisponibilita() {
    free(this->id_indisponibilita);
    free(this->medico_id);
    free(this->inizio);
    free(this->fine);
}

Indisponibilita* Indisponibilita::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    char id_indisponibilita[PARAMETERS_LEN];
    char medico_id[PARAMETERS_LEN];
    char inizio[PARAMETERS_LEN];
    char fine[PARAMETERS_LEN];

    char read_fields = 0b0000;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "id_indisponibilita")) {
            strcpy(id_indisponibilita, value);
            read_fields |= 0b0001;

        } else if (!strcmp(key, "medico_id")) {
            strcpy(medico_id, value);
            read_fields |= 0b0010;

        } else if (!strcmp(key, "inizio")) {
            strcpy(inizio, value);
            read_fields |= 0b0100;

        } else if (!strcmp(key, "fine")) {
            strcpy(fine, value);
            read_fields |= 0b1000;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b1111) {
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new Indisponibilita(id_indisponibilita, medico_id, inizio, fine);
}