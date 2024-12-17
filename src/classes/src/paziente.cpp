#include "paziente.h"
#include <iostream>

Paziente::Paziente( char* cf, char* indirizzo, char* email, char* telefono) {

    this->cf = (char*) malloc(sizeof(char) * PRMTRSIZE);
    this->indirizzo = (char*) malloc(sizeof(char) * PRMTRSIZE);
    this->email = (char*) malloc(sizeof(char) * PRMTRSIZE);
    this->telefono = (char*) malloc(sizeof(char) * PRMTRSIZE);


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
    char key[PRMTRSIZE];
    char value[PRMTRSIZE];


    char cf[PRMTRSIZE];
    char indirizzo[PRMTRSIZE];
    char email[PRMTRSIZE];
    char telefono[PRMTRSIZE];

    char read_fields = 0b00;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "cf")) {
            strncpy(cf, value, PRMTRSIZE - 1);
            cf[PRMTRSIZE - 1] = '\0';
            read_fields |= 0b10;

        } else if (!strcmp(key, "indirizzo")) {
            strncpy(indirizzo, value, PRMTRSIZE - 1);
            indirizzo[PRMTRSIZE - 1] = '\0';
            read_fields |= 0b100;

        } else if (!strcmp(key, "email")) {
            strncpy(email, value, PRMTRSIZE - 1);
            email[PRMTRSIZE - 1] = '\0';
            read_fields |= 0b1000;

        } else if (!strcmp(key, "telefono")) {
            strncpy(telefono, value, PRMTRSIZE - 1);
            telefono[PRMTRSIZE - 1] = '\0';
            read_fields |= 0b10000;

        } else {
            continue;
        }
    }

    if ((read_fields & 0b11110) != 0b11110) {
        std::string missing_fields;
        if (!(read_fields & 0b10)) missing_fields += "cf ";
        if (!(read_fields & 0b100)) missing_fields += "indirizzo ";
        if (!(read_fields & 0b1000)) missing_fields += "email ";
        if (!(read_fields & 0b10000)) missing_fields += "telefono ";
        throw std::invalid_argument("Stream error: missing fields - " + missing_fields);
    }

    return new Paziente( cf, indirizzo, email, telefono);
}

