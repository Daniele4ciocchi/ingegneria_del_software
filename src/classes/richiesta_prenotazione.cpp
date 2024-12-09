#include "richiesta_prenotazione.h"

RichiestaPrenotazione::RichiestaPrenotazione(int id, const char* data, const char* ora, const char* medico_cf, const char* paziente_cf) {
    this->id = id;
    this->data = (char*) malloc(sizeof(char) * 11);
    this->ora = (char*) malloc(sizeof(char) * 6);
    this->medico_cf = (char*) malloc(sizeof(char) * 17);
    this->paziente_cf = (char*) malloc(sizeof(char) * 17);

    strcpy(this->data, data);
    strcpy(this->ora, ora);
    strcpy(this->medico_cf, medico_cf);
    strcpy(this->paziente_cf, paziente_cf);
}

RichiestaPrenotazione::~RichiestaPrenotazione() {
    free(this->data);
    free(this->ora);
    free(this->medico_cf);
    free(this->paziente_cf);
}

RichiestaPrenotazione* RichiestaPrenotazione::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    int id;
    char data[11];
    char ora[6];
    char medico_cf[17];
    char paziente_cf[17];

    char read_fields = 0b00000;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "id")) {
            id = atoi(value);
            read_fields |= 0b00001;

        } else if (!strcmp(key, "data")) {
            snprintf(data, 11, "%s", value);
            read_fields |= 0b00010;

        } else if (!strcmp(key, "ora")) {
            snprintf(ora, 6, "%s", value);
            read_fields |= 0b00100;

        } else if (!strcmp(key, "medico_cf")) {
            snprintf(medico_cf, 17, "%s", value);
            read_fields |= 0b01000;

        } else if (!strcmp(key, "paziente_cf")) {
            snprintf(paziente_cf, 17, "%s", value);
            read_fields |= 0b10000;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b11111) {
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new RichiestaPrenotazione(id, data, ora, medico_cf, paziente_cf);
}