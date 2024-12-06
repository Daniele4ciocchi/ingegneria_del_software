#include "prenotazione_accettata.h"

PrenotazioneAccettata::PrenotazioneAccettata(int richiesta_id, const std::tm& iaccet, bool prestazioneavvenuta) {
    this->richiesta_id = richiesta_id;
    this->iaccet = iaccet;
    this->prestazioneavvenuta = prestazioneavvenuta;
}

PrenotazioneAccettata::~PrenotazioneAccettata() {
    // Destructor logic if needed
}

PrenotazioneAccettata* PrenotazioneAccettata::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    int richiesta_id;
    std::tm iaccet;
    bool prestazioneavvenuta;

    char read_fields = 0b000;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "richiesta_id")) {
            richiesta_id = atoi(value);
            read_fields |= 0b001;

        } else if (!strcmp(key, "iaccet")) {
            strptime(value, "%Y-%m-%d %H:%M:%S", &iaccet);
            read_fields |= 0b010;

        } else if (!strcmp(key, "prestazioneavvenuta")) {
            prestazioneavvenuta = (bool) atoi(value);
            read_fields |= 0b100;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b111) {
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new PrenotazioneAccettata(richiesta_id, iaccet, prestazioneavvenuta);
}