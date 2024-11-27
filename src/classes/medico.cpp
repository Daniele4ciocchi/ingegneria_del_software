#include "medico.h"

Medico::Medico(char* cf_medico, char* nome_medico, char* cognome_medico, char* nascita_medico) {
    cf = (char*) malloc(sizeof(char) * 17);
    nome = (char*) malloc(sizeof(char) * 101);
    cognome = (char*) malloc(sizeof(char) * 101);
    nascita = (char*) malloc(sizeof(char) * 11);

    strcpy(cf, cf_medico);
    strcpy(nome, nome_medico);
    strcpy(cognome, cognome_medico);
    strcpy(nascita, nascita_medico);
}

Medico::~Medico() {
    free(cf);
    free(nome);
    free(cognome);
    free(nascita);
}

Medico* Medico::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    char cf[17];
    char nome[101];
    char cognome[101];
    char nascita[11];

    char read_fields = 0b0000;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "cf")) {
            snprintf(cf, 17, "%s", value);
            read_fields |= 0b0001;

        } else if (!strcmp(key, "nome")) {
            snprintf(nome, 101, "%s", value);
            read_fields |= 0b0010;

        } else if (!strcmp(key, "cognome")) {
            snprintf(cognome, 101, "%s", value);
            read_fields |= 0b0100;

        } else if (!strcmp(key, "nascita")) {
            snprintf(nascita, 11, "%s", value);
            read_fields |= 0b1000;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b1111) {
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new Medico(cf, nome, cognome, nascita);
}
