#include "prenotazione_rifiutata.h"

PrenotazioneRifiutata::PrenotazioneRifiutata(char* richiesta_id, char* irif, char* motivazione_id) {
    this->richiesta_id = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->irif = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->motivazione_id = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(this->richiesta_id, richiesta_id);
    strcpy(this->irif, irif);
    strcpy(this->motivazione_id, motivazione_id);
}

PrenotazioneRifiutata::~PrenotazioneRifiutata() {
    free(this->richiesta_id);
    free(this->irif);
    free(this->motivazione_id);
}

PrenotazioneRifiutata* PrenotazioneRifiutata::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    char richiesta_id[PARAMETERS_LEN];
    char* irif;
    char motivazione_id[PARAMETERS_LEN];

    char read_fields = 0b00;

    auto current_time = std::chrono::system_clock::now();
    std::time_t current_time_t = std::chrono::system_clock::to_time_t(current_time);
    irif = std::ctime(&current_time_t);

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "richiesta_id")) {
            strcpy(richiesta_id, value);
            read_fields |= 0b01;

        }else if (!strcmp(key, "motivazione_id")) {
            strcpy(motivazione_id, value);
            read_fields |= 0b10;
        }

    }

    if (read_fields != 0b11) {
        throw std::invalid_argument("Missing fields in PrenotazioneRifiutata");
    }

    return new PrenotazioneRifiutata(richiesta_id, irif, motivazione_id);
}