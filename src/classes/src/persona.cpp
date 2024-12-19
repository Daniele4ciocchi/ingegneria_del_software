#include "persona.h"
#include <iostream>

Persona::Persona(char *cf, char *nome, char *cognome, char *nascita)
{

    this->cf = (char *)malloc(sizeof(char) * PARAMETERS_LEN);
    this->nome = (char *)malloc(sizeof(char) * PARAMETERS_LEN);
    this->cognome = (char *)malloc(sizeof(char) * PARAMETERS_LEN);
    this->nascita = (char *)malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(this->cf, cf);
    strcpy(this->nome, nome);
    strcpy(this->cognome, cognome);
    strcpy(this->nascita, nascita);
}

Persona::~Persona()
{

    free(this->cf);
    free(this->nome);
    free(this->cognome);
    free(this->nascita);
}

Persona *Persona::from_stream(redisReply *reply, int stream_num, int msg_num)
{
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    char cf[PARAMETERS_LEN];
    char nome[PARAMETERS_LEN];
    char cognome[PARAMETERS_LEN];
    char nascita[PARAMETERS_LEN];

    char read_fields = 0b00;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2)
    {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "cf"))
        {
            strncpy(cf, value, PARAMETERS_LEN - 1);
            cf[PARAMETERS_LEN - 1] = '\0';
            read_fields |= 0b10;
        }
        else if (!strcmp(key, "nome"))
        {
            strncpy(nome, value, PARAMETERS_LEN - 1);
            nome[PARAMETERS_LEN - 1] = '\0';
            read_fields |= 0b100;
        }
        else if (!strcmp(key, "cognome"))
        {
            strncpy(cognome, value, PARAMETERS_LEN - 1);
            cognome[PARAMETERS_LEN - 1] = '\0';
            read_fields |= 0b1000;
        }
        else if (!strcmp(key, "nascita"))
        {
            strncpy(nascita, value, PARAMETERS_LEN - 1);
            nascita[PARAMETERS_LEN - 1] = '\0';
            read_fields |= 0b10000;
        }
        else
        {
            continue; // Ignora campi sconosciuti
        }
    }

    if ((read_fields & 0b11110) != 0b11110)
    {
        std::string missing_fields;
        if (!(read_fields & 0b10))
            missing_fields += "cf ";
        if (!(read_fields & 0b100))
            missing_fields += "nome ";
        if (!(read_fields & 0b1000))
            missing_fields += "cognome ";
        if (!(read_fields & 0b10000))
            missing_fields += "nascita ";
        throw std::invalid_argument("Stream error: missing fields - " + missing_fields);
    }

    return new Persona(cf, nome, cognome, nascita);
}
