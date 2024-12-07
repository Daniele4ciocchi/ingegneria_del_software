#include "prenotazione_rifiutata.h"

PrenotazioneRifiutata::PrenotazioneRifiutata(int id, char* data, char* ora, char* medico_cf, char* paziente_cf, char* motivo) {
    this->id = id;
    this->data = (char*) malloc(sizeof(char) * 11);
    this->ora = (char*) malloc(sizeof(char) * 6);
    this->medico_cf = (char*) malloc(sizeof(char) * 17);
    this->paziente_cf = (char*) malloc(sizeof(char) * 17);
    this->motivo = (char*) malloc(sizeof(char) * 256);

    strcpy(this->data, data);
    strcpy(this->ora, ora);
    strcpy(this->medico_cf, medico_cf);
    strcpy(this->paziente_cf, paziente_cf);
    strcpy(this->motivo, motivo);
}

PrenotazioneRifiutata::~PrenotazioneRifiutata() {
    free(this->data);
    free(this->ora);
    free(this->medico_cf);
    free(this->paziente_cf);
    free(this->motivo);
}

PrenotazioneRifiutata* PrenotazioneRifiutata::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    int id;
    char data[11];
    char ora[6];
    char medico_cf[17];
    char paziente_cf[17];
    char motivo[256];

    char read_fields = 0b000000;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "id")) {
            id = atoi(value);
            read_fields |= 0b000001;

        } else if (!strcmp(key, "data")) {
            snprintf(data, 11, "%s", value);
            read_fields |= 0b000010;

        } else if (!strcmp(key, "ora")) {
            snprintf(ora, 6, "%s", value);
            read_fields |= 0b000100;

        } else if (!strcmp(key, "medico_cf")) {
            snprintf(medico_cf, 17, "%s", value);
            read_fields |= 0b001000;

        } else if (!strcmp(key, "paziente_cf")) {
            snprintf(paziente_cf, 17, "%s", value);
            read_fields |= 0b010000;

        } else if (!strcmp(key, "motivo")) {
            snprintf(motivo, 256, "%s", value);
            read_fields |= 0b100000;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b111111) {
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new PrenotazioneRifiutata(id, data, ora, medico_cf, paziente_cf, motivo);
}