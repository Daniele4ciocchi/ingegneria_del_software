#include "motivazione.h"

Motivazione::Motivazione(int id, const char* motivo) {
    this->id = id;
    this->motivo = (char*) malloc(sizeof(char) * 101);
    strcpy(this->motivo, motivo);
}

Motivazione::~Motivazione() {
    free(this->motivo);
}

Motivazione* Motivazione::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    int id;
    char motivo[101];

    char read_fields = 0b00;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "id")) {
            id = atoi(value);
            read_fields |= 0b01;

        } else if (!strcmp(key, "motivo")) {
            snprintf(motivo, 101, "%s", value);
            read_fields |= 0b10;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b11) {
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new Motivazione(id, motivo);
}