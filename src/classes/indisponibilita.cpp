#include "indisponibilita.h"

Indisponibilita::Indisponibilita(int id, int medico_id, const std::tm& inizioind, const std::tm& fineind) {
    this->id = id;
    this->medico_id = medico_id;
    this->inizioind = inizioind;
    this->fineind = fineind;
}

Indisponibilita::~Indisponibilita() {
    // Destructor logic if needed
}

Indisponibilita* Indisponibilita::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    int id;
    int medico_id;
    std::tm inizioind;
    std::tm fineind;

    char read_fields = 0b0000;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "id")) {
            id = atoi(value);
            read_fields |= 0b0001;

        } else if (!strcmp(key, "medico_id")) {
            medico_id = atoi(value);
            read_fields |= 0b0010;

        } else if (!strcmp(key, "inizioind")) {
            strptime(value, "%Y-%m-%d %H:%M:%S", &inizioind);
            read_fields |= 0b0100;

        } else if (!strcmp(key, "fineind")) {
            strptime(value, "%Y-%m-%d %H:%M:%S", &fineind);
            read_fields |= 0b1000;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b1111) {
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new Indisponibilita(id, medico_id, inizioind, fineind);
}