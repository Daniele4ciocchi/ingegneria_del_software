#include "paziente.h"

Paziente::Paziente( char* cf, char* indirizzo, char* email, char* telefono) {

    this->cf = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->indirizzo = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->email = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->telefono = (char*) malloc(sizeof(char) * PARAMETERS_LEN);


    strcpy(this->cf, cf);
    strcpy(this->indirizzo, indirizzo);
    strcpy(this->email, email);
    strcpy(this->telefono, telefono);
}

Paziente::~Paziente() {

    free(this->cf);
    free(this->indirizzo);
    free(this->email);
    free(this->telefono);
}

Paziente* Paziente::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];


    char cf[PARAMETERS_LEN];
    char indirizzo[PARAMETERS_LEN];
    char email[PARAMETERS_LEN];
    char telefono[PARAMETERS_LEN];

    char read_fields = 0b00;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "cf")) {
            strcpy(cf, value);
            read_fields |= 0b10;

        } else if (!strcmp(key, "indirizzo")) {
            strcpy(indirizzo, value);
            read_fields |= 0b100;

        } else if (!strcmp(key, "email")) {
            strcpy(email, value);
            read_fields |= 0b1000;

        } else if (!strcmp(key, "telefono")) {
            strcpy(telefono, value);
            read_fields |= 0b10000;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    return new Paziente( cf, indirizzo, email, telefono);
}

