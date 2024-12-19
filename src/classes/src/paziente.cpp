#include "paziente.h"
#include <iostream>

Paziente::Paziente(char *cf, char *indirizzo, char *email, char *telefono)
{

    this->cf = (char *)malloc(sizeof(char) * PARAMETERS_LEN);
    this->indirizzo = (char *)malloc(sizeof(char) * PARAMETERS_LEN);
    this->email = (char *)malloc(sizeof(char) * PARAMETERS_LEN);
    this->telefono = (char *)malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(this->cf, cf);
    strcpy(this->indirizzo, indirizzo);
    strcpy(this->email, email);
    strcpy(this->telefono, telefono);
}

Paziente::~Paziente()
{

    free(this->cf);
    free(this->indirizzo);
    free(this->email);
    free(this->telefono);
}

Paziente *Paziente::from_stream(redisReply *reply, int stream_num, int msg_num)
{
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    char cf[PARAMETERS_LEN];
    char indirizzo[PARAMETERS_LEN];
    char email[PARAMETERS_LEN];
    char telefono[PARAMETERS_LEN];

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
        else if (!strcmp(key, "indirizzo"))
        {
            strncpy(indirizzo, value, PARAMETERS_LEN - 1);
            indirizzo[PARAMETERS_LEN - 1] = '\0';
            read_fields |= 0b100;
        }
        else if (!strcmp(key, "email"))
        {
            strncpy(email, value, PARAMETERS_LEN - 1);
            email[PARAMETERS_LEN - 1] = '\0';
            read_fields |= 0b1000;
        }
        else if (!strcmp(key, "telefono"))
        {
            strncpy(telefono, value, PARAMETERS_LEN - 1);
            telefono[PARAMETERS_LEN - 1] = '\0';
            read_fields |= 0b10000;
        }
        else
        {
            continue;
        }
    }

    if ((read_fields & 0b11110) != 0b11110)
    {
        std::string missing_fields;
        if (!(read_fields & 0b10))
            missing_fields += "cf ";
        if (!(read_fields & 0b100))
            missing_fields += "indirizzo ";
        if (!(read_fields & 0b1000))
            missing_fields += "email ";
        if (!(read_fields & 0b10000))
            missing_fields += "telefono ";
        throw std::invalid_argument("Stream error: missing fields - " + missing_fields);
    }

    return new Paziente(cf, indirizzo, email, telefono);
}
