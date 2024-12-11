#include "persona.h"

Persona::Persona( char* cf, char* nome, char* cognome, char* nascita) {

    this->cf = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->nome = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->cognome = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->nascita = (char*) malloc(sizeof(char) * PARAMETERS_LEN);


    strcpy(this->cf, cf);
    strcpy(this->nome, nome);
    strcpy(this->cognome, cognome);
    strcpy(this->nascita, nascita);
}

Persona::~Persona() {
    
    free(this->cf);
    free(this->nome);
    free(this->cognome);
    free(this->nascita);
}

Persona* Persona::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];


    char cf[PARAMETERS_LEN];
    char nome[PARAMETERS_LEN];
    char cognome[PARAMETERS_LEN];
    char nascita[PARAMETERS_LEN];

    char read_fields = 0b00;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "cf")) {
            strcpy(cf, value);
            read_fields |= 0b10;

        } else if (!strcmp(key, "nome")) {
            strcpy(nome, value);
            read_fields |= 0b100;

        } else if (!strcmp(key, "cognome")) {
            strcpy(cognome, value);
            read_fields |= 0b1000;

        } else if (!strcmp(key, "nascita")) {
            strcpy(nascita, value);
            read_fields |= 0b10000;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    return new Persona( cf, nome, cognome, nascita);
}

