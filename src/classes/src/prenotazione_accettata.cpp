#include "prenotazione_accettata.h"

PrenotazioneAccettata::PrenotazioneAccettata(char *richiesta_id, char *iaccet, char *prestazioneavvenuta)
{
    this->richiesta_id = (char *)malloc(sizeof(char) * PARAMETERS_LEN);
    this->iaccet = (char *)malloc(sizeof(char) * PARAMETERS_LEN);
    this->prestazioneavvenuta = (char *)malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(this->richiesta_id, richiesta_id);
    strcpy(this->iaccet, iaccet);
    strcpy(this->prestazioneavvenuta, prestazioneavvenuta);
}

PrenotazioneAccettata::~PrenotazioneAccettata()
{
    free(this->richiesta_id);
    free(this->iaccet);
    free(this->prestazioneavvenuta);
}

PrenotazioneAccettata *PrenotazioneAccettata::from_stream(redisReply *reply, int stream_num, int msg_num)
{
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    char richiesta_id[PARAMETERS_LEN];
    char *iaccet;
    char prestazioneavvenuta[PARAMETERS_LEN];

    char read_fields = 0b0;

    auto current_time = std::chrono::system_clock::now();
    std::time_t current_time_t = std::chrono::system_clock::to_time_t(current_time);
    iaccet = std::ctime(&current_time_t);

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2)
    {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "richiesta_id"))
        {
            strcpy(richiesta_id, value);
            read_fields |= 0b1;
        }

        strcpy(prestazioneavvenuta, "false");
    }

    if (read_fields != 0b1)
    {
        throw std::invalid_argument("Missing fields in PrenotazioneAccettata");
    }

    return new PrenotazioneAccettata(richiesta_id, iaccet, prestazioneavvenuta);
}
