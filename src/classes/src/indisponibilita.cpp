#include "indisponibilita.h"

Indisponibilita::Indisponibilita( char* medico_id, char* inizio, char* fine) {

    this->medico_id = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->inizio = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->fine = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    
    strcpy(this->medico_id, medico_id);
    strcpy(this->inizio, inizio);
    strcpy(this->fine, fine);

}

Indisponibilita::~Indisponibilita() {

    free(this->medico_id);
    free(this->inizio);
    free(this->fine);
}

void replaceDoubleHashWithSpace(char* str) {
    char* pos;
    while ((pos = strstr(str, "##")) != nullptr) {
        *pos = ' ';
        memmove(pos + 1, pos + 2, strlen(pos + 2) + 1); // Shift remaining characters left
    }
}

Indisponibilita* Indisponibilita::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    char medico_id[PARAMETERS_LEN];
    char inizio[PARAMETERS_LEN];
    char fine[PARAMETERS_LEN];

    char read_fields = 0b000;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "medico_id")) {
            strcpy(medico_id, value);
            read_fields |= 0b001;

        } else if (!strcmp(key, "inizioind")) {
            strcpy(inizio, value);
            //replaceDoubleHashWithSpace(inizio);
            read_fields |= 0b010;

        } else if (!strcmp(key, "fineind")) {
            strcpy(fine, value);
            //replaceDoubleHashWithSpace(fine);
            read_fields |= 0b100;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b111) {
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new Indisponibilita(medico_id, inizio, fine);
}