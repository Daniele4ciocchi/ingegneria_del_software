#include "paziente.h"

Paziente::Paziente( char* cf, char* email, char* telefono, char* via, char* numero_civico, char* cap, char* citta, char* provincia) {

    this->cf = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->email = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->telefono = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->via = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->numero_civico = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->cap = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->citta = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->provincia = (char*) malloc(sizeof(char) * PARAMETERS_LEN);


    strcpy(this->cf, cf);
    strcpy(this->email, email);
    strcpy(this->telefono, telefono);
    strcpy(this->via, via);
    strcpy(this->numero_civico, numero_civico);
    strcpy(this->cap, cap);
    strcpy(this->citta, citta);
    strcpy(this->provincia, provincia);
}

Paziente::~Paziente() {

    free(this->cf);
    free(this->email);
    free(this->telefono);
    free(this->via);
    free(this->numero_civico);
    free(this->cap);
    free(this->citta);
    free(this->provincia);
}

Paziente* Paziente::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];


    char cf[PARAMETERS_LEN];
    char email[PARAMETERS_LEN];
    char telefono[PARAMETERS_LEN];
    char via[PARAMETERS_LEN];
    char numero_civico[PARAMETERS_LEN];
    char cap[PARAMETERS_LEN];
    char citta[PARAMETERS_LEN];
    char provincia[PARAMETERS_LEN];

    char read_fields = 0b00;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (strcmp(key, "cf") == 0) {
            strcpy(cf, value);
            read_fields |= 0b01;
        } else if (strcmp(key, "email") == 0) {
            strcpy(email, value);
            read_fields |= 0b10;
        } else if (strcmp(key, "telefono") == 0) {
            strcpy(telefono, value);
            read_fields |= 0b100;
        } else if (strcmp(key, "via") == 0) {
            strcpy(via, value);
            read_fields |= 0b1000;
        } else if (strcmp(key, "numero_civico") == 0) {
            strcpy(numero_civico, value);
            read_fields |= 0b10000;
        } else if (strcmp(key, "cap") == 0) {
            strcpy(cap, value);
            read_fields |= 0b100000;
        } else if (strcmp(key, "citta") == 0) {
            strcpy(citta, value);
            read_fields |= 0b1000000;
        } else if (strcmp(key, "provincia") == 0) {
            strcpy(provincia, value);
            read_fields |= 0b10000000;
        }
    }

    return new Paziente( cf, email, telefono, via, numero_civico, cap, citta, provincia);
}

