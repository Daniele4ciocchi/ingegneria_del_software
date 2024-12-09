#include "richiesta_prenotazione.h"

<<<<<<< HEAD
RichiestaPrenotazione::RichiestaPrenotazione(char* id, char* paziente_id, char* medico_id, char* amministrativo_id, char* specializzazione_nome, char* giornoorariopren) {
    
    this->id = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->paziente_id = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->medico_id = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->amministrativo_id = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->specializzazione_nome = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->giornoorariopren = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(this->id, id);
    strcpy(this->paziente_id, paziente_id);
    strcpy(this->medico_id, medico_id);
    strcpy(this->amministrativo_id, amministrativo_id);
    strcpy(this->specializzazione_nome, specializzazione_nome);
    strcpy(this->giornoorariopren, giornoorariopren);

}

RichiestaPrenotazione::~RichiestaPrenotazione() {
    free(this->id);
    free(this->paziente_id);
    free(this->medico_id);
    free(this->amministrativo_id);
    free(this->specializzazione_nome);
    free(this->giornoorariopren);
=======
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
>>>>>>> refs/remotes/origin/main
}

RichiestaPrenotazione* RichiestaPrenotazione::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

<<<<<<< HEAD
    char id[PARAMETERS_LEN];
    char paziente_id[PARAMETERS_LEN];
    char medico_id[PARAMETERS_LEN];
    char amministrativo_id[PARAMETERS_LEN];
    char specializzazione_nome[PARAMETERS_LEN];
    char giornoorariopren[PARAMETERS_LEN];

    char read_fields = 0b00;
=======
    int id;
    char data[11];
    char ora[6];
    char medico_cf[17];
    char paziente_cf[17];

    char read_fields = 0b00000;
>>>>>>> refs/remotes/origin/main

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "id")) {
<<<<<<< HEAD
            strcpy(id, value);
            read_fields |= 0b01;

        } else if (!strcmp(key, "paziente_id")) {
            strcpy(paziente_id, value);
            read_fields |= 0b10;

        } else if (!strcmp(key, "medico_id")) {
            strcpy(medico_id, value);
            read_fields |= 0b100;

        } else if (!strcmp(key, "amministrativo_id")) {
            strcpy(amministrativo_id, value);
            read_fields |= 0b1000;

        } else if (!strcmp(key, "specializzazione_nome")) {
            strcpy(specializzazione_nome, value);
            read_fields |= 0b10000;

        } else if (!strcmp(key, "giornoorariopren")) {
            strcpy(giornoorariopren, value);
            read_fields |= 0b100000;
        }
    }

    if (read_fields != 0b111111) {
        throw std::invalid_argument("Missing fields in RichiestaPrenotazione");
    }

    return new RichiestaPrenotazione(id, paziente_id, medico_id, amministrativo_id, specializzazione_nome, giornoorariopren);
=======
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
>>>>>>> refs/remotes/origin/main
}