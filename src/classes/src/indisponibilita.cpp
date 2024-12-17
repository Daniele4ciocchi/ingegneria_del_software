#include "indisponibilita.h"

Indisponibilita::Indisponibilita( char* medico_id, char* inizio, char* fine) {

    this->medico_id = (char*) malloc(sizeof(char) * PRMTRSIZE);
    this->inizio = (char*) malloc(sizeof(char) * PRMTRSIZE);
    this->fine = (char*) malloc(sizeof(char) * PRMTRSIZE);
    
    strcpy(this->medico_id, medico_id);
    strcpy(this->inizio, inizio);
    strcpy(this->fine, fine);

}

Indisponibilita::~Indisponibilita() {

    free(this->medico_id);
    free(this->inizio);
    free(this->fine);
}

Indisponibilita* Indisponibilita::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PRMTRSIZE];
    char value[PRMTRSIZE];

    char medico_id[PRMTRSIZE];
    char inizio[PRMTRSIZE];
    char fine[PRMTRSIZE];

    char read_fields = 0b0000;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "medico_id")) {
            strcpy(medico_id, value);
            read_fields |= 0b0010;

        } else if (!strcmp(key, "inizioind")) {
            strcpy(inizio, value);
            read_fields |= 0b0100;

        } else if (!strcmp(key, "fineind")) {
            strcpy(fine, value);
            read_fields |= 0b1000;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b1111) {
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new Indisponibilita( medico_id, inizio, fine);
}