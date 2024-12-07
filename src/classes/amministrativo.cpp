#include "amministrativo.h"

Amministrativo::Amministrativo(char* cf_amministrativo,
        char* nome_amministrativo,
        char* cognome_amministrativo,
        char* nascita_amministrativo) {

    cf = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    nome = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    cognome = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    nascita = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(cf, cf_amministrativo);
    strcpy(nome, nome_amministrativo);
    strcpy(cognome, cognome_amministrativo);
    strcpy(nascita, nascita_amministrativo);
}

Amministrativo::~Amministrativo() {
    free(cf);
    free(nome);
    free(cognome);
    free(nascita);
}

Amministrativo* Amministrativo::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    char cf[PARAMETERS_LEN];
    char nome[PARAMETERS_LEN];
    char cognome[PARAMETERS_LEN];
    char nascita[PARAMETERS_LEN];

    char read_fields = 0b0000;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "cf")) {
            snprintf(cf, PARAMETERS_LEN, "%s", value);
            read_fields |= 0b0001;

        } else if (!strcmp(key, "nome")) {
            snprintf(nome, PARAMETERS_LEN, "%s", value);
            read_fields |= 0b0010;

        } else if (!strcmp(key, "cognome")) {
            snprintf(cognome, PARAMETERS_LEN, "%s", value);
            read_fields |= 0b0100;

        } else if (!strcmp(key, "nascita")) {
            snprintf(nascita, PARAMETERS_LEN, "%s", value);
            read_fields |= 0b1000;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b1111) {
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new Amministrativo(cf, nome, cognome, nascita);
}