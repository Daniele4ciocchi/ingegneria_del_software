#include "motivazione.h"

Motivazione::Motivazione(char* id, char* motivo) {
    this->id = (char*) malloc(sizeof(char) * PRMTRSIZE);
    this->motivo = (char*) malloc(sizeof(char) * PRMTRSIZE);

    strcpy(this->motivo, motivo);
    strcpy(this->id, id);
}

Motivazione::~Motivazione() {
    free(this->id);
    free(this->motivo);
}

Motivazione* Motivazione::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PRMTRSIZE];
    char value[PRMTRSIZE];

    char id[PRMTRSIZE];
    char motivo[PRMTRSIZE];

    char read_fields = 0b00;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "id")) {
            strcpy(id, value);
            read_fields |= 0b01;

        } else if (!strcmp(key, "motivo")) {
            strcpy(motivo, value);
            read_fields |= 0b10;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    return new Motivazione(id, motivo);
}