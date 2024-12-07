#include "medico.h"

Medico::Medico(int id, char* nome, char* cognome, char* specializzazione) {
    this->id = id;
    this->nome = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->cognome = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->specializzazione = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(this->nome, nome);
    strcpy(this->cognome, cognome);
    strcpy(this->specializzazione, specializzazione);
}

Medico::~Medico() {
    free(nome);
    free(cognome);
    free(specializzazione);
}

Medico* Medico::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    int id;
    char nome[PARAMETERS_LEN];
    char cognome[PARAMETERS_LEN];
    char specializzazione[PARAMETERS_LEN];

    char read_fields = 0b0000;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "id")) {
            id = atoi(value);
            read_fields |= 0b0001;

        } else if (!strcmp(key, "nome")) {
            snprintf(nome, PARAMETERS_LEN, "%s", value);
            read_fields |= 0b0010;

        } else if (!strcmp(key, "cognome")) {
            snprintf(cognome, PARAMETERS_LEN, "%s", value);
            read_fields |= 0b0100;

        } else if (!strcmp(key, "specializzazione")) {
            snprintf(specializzazione, PARAMETERS_LEN, "%s", value);
            read_fields |= 0b1000;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b1111) {
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new Medico(id, nome, cognome, specializzazione);
}