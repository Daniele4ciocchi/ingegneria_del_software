#include "paziente.h"

Paziente::Paziente(char* cf_paziente, char* nome_paziente, char* cognome_paziente, char* nascita_paziente, char* indirizzo_paziente, char* email_paziente, char* telefono_paziente) {
    cf = (char*) malloc(sizeof(char) * 17);
    nome = (char*) malloc(sizeof(char) * 101);
    cognome = (char*) malloc(sizeof(char) * 101);
    nascita = (char*) malloc(sizeof(char) * 11);
    indirizzo = (char*) malloc(sizeof(char) * 256);
    email = (char*) malloc(sizeof(char) * 101);
    telefono = (char*) malloc(sizeof(char) * 16);

    strcpy(cf, cf_paziente);
    strcpy(nome, nome_paziente);
    strcpy(cognome, cognome_paziente);
    strcpy(nascita, nascita_paziente);
    strcpy(indirizzo, indirizzo_paziente);
    strcpy(email, email_paziente);
    strcpy(telefono, telefono_paziente);
}

Paziente::~Paziente() {
    free(cf);
    free(nome);
    free(cognome);
    free(nascita);
    free(indirizzo);
    free(email);
    free(telefono);
}

Paziente* Paziente::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    char cf[17];
    char nome[101];
    char cognome[101];
    char nascita[11];
    char indirizzo[256];
    char email[101];
    char telefono[16];

    char read_fields = 0b0000000;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "cf")) {
            snprintf(cf, 17, "%s", value);
            read_fields |= 0b0000001;

        } else if (!strcmp(key, "nome")) {
            snprintf(nome, 101, "%s", value);
            read_fields |= 0b0000010;

        } else if (!strcmp(key, "cognome")) {
            snprintf(cognome, 101, "%s", value);
            read_fields |= 0b0000100;

        } else if (!strcmp(key, "nascita")) {
            snprintf(nascita, 11, "%s", value);
            read_fields |= 0b0001000;

        } else if (!strcmp(key, "indirizzo")) {
            snprintf(indirizzo, 256, "%s", value);
            read_fields |= 0b0010000;

        } else if (!strcmp(key, "email")) {
            snprintf(email, 101, "%s", value);
            read_fields |= 0b0100000;

        } else if (!strcmp(key, "telefono")) {
            snprintf(telefono, 16, "%s", value);
            read_fields |= 0b1000000;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b1111111) {
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new Paziente(cf, nome, cognome, nascita, indirizzo, email, telefono);
}