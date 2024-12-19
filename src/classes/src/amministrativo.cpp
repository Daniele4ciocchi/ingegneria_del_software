#include "amministrativo.h"

Amministrativo::Amministrativo(char *id_amministrativo, char *cf_amministrativo)
{

    this->id_amministrativo = (char *)malloc(sizeof(char) * PARAMETERS_LEN);
    this->cf_amministrativo = (char *)malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(this->id_amministrativo, id_amministrativo);
    strcpy(this->cf_amministrativo, cf_amministrativo);
}

Amministrativo::~Amministrativo()
{
    free(this->id_amministrativo);
    free(this->cf_amministrativo);
}

Amministrativo *Amministrativo::from_stream(redisReply *reply, int stream_num, int msg_num)
{
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    char id_amministrativo[PARAMETERS_LEN];
    char cf_amministrativo[PARAMETERS_LEN];

    char read_fields = 0b00;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2)
    {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "id_amministrativo"))
        {
            strcpy(id_amministrativo, value);
            read_fields |= 0b01;
        }
        else if (!strcmp(key, "cf_amministrativo"))
        {
            strcpy(cf_amministrativo, value);
            read_fields |= 0b10;
        }
        else
        {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b11)
    {
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new Amministrativo(id_amministrativo, cf_amministrativo);
}