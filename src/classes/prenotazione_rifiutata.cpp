#include "prenotazione_rifiutata.h"

PrenotazioneRifiutata::PrenotazioneRifiutata(int richiesta_id, const std::tm& irif, int motivazione_id) {
    this->richiesta_id = richiesta_id;
    this->irif = irif;
    this->motivazione_id = motivazione_id;
}

PrenotazioneRifiutata::~PrenotazioneRifiutata() {
    // Destructor logic if needed
}

PrenotazioneRifiutata* PrenotazioneRifiutata::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    int richiesta_id;
    std::tm irif;
    int motivazione_id;

    char read_fields = 0b000;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "richiesta_id")) {
            richiesta_id = atoi(value);
            read_fields |= 0b001;

        } else if (!strcmp(key, "irif")) {
            strptime(value, "%Y-%m-%d %H:%M:%S", &irif);
            read_fields |= 0b010;

        } else if (!strcmp(key, "motivazione_id")) {
            motivazione_id = atoi(value);
            read_fields |= 0b100;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b111) {
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new PrenotazioneRifiutata(richiesta_id, irif, motivazione_id);
}