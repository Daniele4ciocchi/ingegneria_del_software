#include "indisponibilita.h"

Indisponibilita::Indisponibilita(int medico_id, const std::tm& inizio, const std::tm& fine) {
    this->medico_id = medico_id;
    this->inizio = inizio;
    this->fine = fine;
}

Indisponibilita::~Indisponibilita() {
    // Destructor logic if needed
}

Indisponibilita* Indisponibilita::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    int medico_id;
    std::tm inizio;
    std::tm fine;

    char read_fields = 0b000;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "medico_id")) {
            medico_id = atoi(value);
            read_fields |= 0b001;

        } else if (!strcmp(key, "inizio")) {
            strptime(value, "%Y-%m-%d %H:%M:%S", &inizio);
            read_fields |= 0b010;

        } else if (!strcmp(key, "fine")) {
            strptime(value, "%Y-%m-%d %H:%M:%S", &fine);
            read_fields |= 0b100;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b111) {
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new Indisponibilita(medico_id, inizio, fine);
}