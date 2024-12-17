#include "feedback.h"

Feedback::Feedback(char* paziente_id, char* prenotazione_accettata_id, char* ifeed,  char* votosodd, char* votopunt) {

    this->paziente_id = (char*) malloc(sizeof(char) * PRMTRSIZE);
    this->prenotazione_accettata_id = (char*) malloc(sizeof(char) * PRMTRSIZE);
    this->ifeed = (char*) malloc(sizeof(char) * PRMTRSIZE);
    this->votosodd = (char*) malloc(sizeof(char) * PRMTRSIZE);
    this->votopunt = (char*) malloc(sizeof(char) * PRMTRSIZE);

    strcpy(this->paziente_id, paziente_id);
    strcpy(this->prenotazione_accettata_id, prenotazione_accettata_id);
    strcpy(this->ifeed, ifeed);
    strcpy(this->votosodd, votosodd);
    strcpy(this->votopunt, votopunt);
}

Feedback::~Feedback() {
    free(this->paziente_id);
    free(this->prenotazione_accettata_id);
    free(this->ifeed);
    free(this->votosodd);
    free(this->votopunt);
}

Feedback* Feedback::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PRMTRSIZE];
    char value[PRMTRSIZE];

    char paziente_id[PRMTRSIZE];
    char prenotazione_accettata_id[PRMTRSIZE];
    char* ifeed;
    char votosodd[PRMTRSIZE];
    char votopunt[PRMTRSIZE];

    char read_fields = 0b00000;
    auto current_time = std::chrono::system_clock::now();
    std::time_t current_time_t = std::chrono::system_clock::to_time_t(current_time);
    ifeed = std::ctime(&current_time_t);

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "paziente_id")) {
            strcpy(paziente_id, value);
            read_fields |= 0b0001;

        } else if (!strcmp(key, "prenotazione_accettata_id")) {
            strcpy(prenotazione_accettata_id, value);
            read_fields |= 0b00010;

        }else if (!strcmp(key, "votosodd")) {
            strcpy(votosodd, value);
            read_fields |= 0b0100;

        } else if (!strcmp(key, "votopunt")) {
            strcpy(votopunt, value);
            read_fields |= 0b1000;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b1111) {
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new Feedback(paziente_id, prenotazione_accettata_id, ifeed, votosodd, votopunt);
}

