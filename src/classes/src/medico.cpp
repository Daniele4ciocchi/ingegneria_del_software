#include "medico.h"

Medico::Medico(char* id,char* cf ) {
    this->id = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->cf = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(this->id, id);
    strcpy(this->cf, cf);
}

Medico::~Medico() {
    free(this->id);
    free(this->cf);
}


Medico* Medico::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    char id[PARAMETERS_LEN];
    char cf[PARAMETERS_LEN];

    char read_fields = 0b00;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "id")) {
            strcpy(id, value);
            read_fields |= 0b01;

        } else if (!strcmp(key, "cf")) {
            strcpy(cf, value);
            read_fields |= 0b10;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    return new Medico(id, cf);
}