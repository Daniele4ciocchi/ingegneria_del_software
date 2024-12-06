#include "feedback.h"

Feedback::Feedback(int paziente_id, int prenotazione_accettata_id, const std::tm& ifeed, int votosodd, int votopunt) {
    this->paziente_id = paziente_id;
    this->prenotazione_accettata_id = prenotazione_accettata_id;
    this->ifeed = ifeed;
    this->votosodd = votosodd;
    this->votopunt = votopunt;
}

Feedback::~Feedback() {
    // Destructor logic if needed
}

Feedback* Feedback::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    int paziente_id;
    int prenotazione_accettata_id;
    std::tm ifeed;
    int votosodd;
    int votopunt;

    char read_fields = 0b00000;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "paziente_id")) {
            paziente_id = atoi(value);
            read_fields |= 0b00001;

        } else if (!strcmp(key, "prenotazione_accettata_id")) {
            prenotazione_accettata_id = atoi(value);
            read_fields |= 0b00010;

        } else if (!strcmp(key, "ifeed")) {
            strptime(value, "%Y-%m-%d %H:%M:%S", &ifeed);
            read_fields |= 0b00100;

        } else if (!strcmp(key, "votosodd")) {
            votosodd = atoi(value);
            read_fields |= 0b01000;

        } else if (!strcmp(key, "votopunt")) {
            votopunt = atoi(value);
            read_fields |= 0b10000;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b11111) {
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new Feedback(paziente_id, prenotazione_accettata_id, ifeed, votosodd, votopunt);
}