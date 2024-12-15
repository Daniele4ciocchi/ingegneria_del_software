#include "richiesta_prenotazione.h"

RichiestaPrenotazione::RichiestaPrenotazione( char* paziente_id, char* medico_id, char* amministrativo_id, char* specializzazione_nome, char* irich, char* giornoorariopren) {
    
    this->paziente_id = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->medico_id = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->amministrativo_id = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->specializzazione_nome = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->irich = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->giornoorariopren = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(this->paziente_id, paziente_id);
    strcpy(this->medico_id, medico_id);
    strcpy(this->amministrativo_id, amministrativo_id);
    strcpy(this->specializzazione_nome, specializzazione_nome);
    strcpy(this->irich, irich);
    strcpy(this->giornoorariopren, giornoorariopren);

}

RichiestaPrenotazione::~RichiestaPrenotazione() {
    free(this->paziente_id);
    free(this->medico_id);
    free(this->amministrativo_id);
    free(this->specializzazione_nome);
    free(this->irich);
    free(this->giornoorariopren);
}

RichiestaPrenotazione* RichiestaPrenotazione::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    char paziente_id[PARAMETERS_LEN];
    char medico_id[PARAMETERS_LEN];
    char amministrativo_id[PARAMETERS_LEN];
    char specializzazione_nome[PARAMETERS_LEN];
    char* irich;
    char giornoorariopren[PARAMETERS_LEN];

    char read_fields = 0b00;
    auto current_time = std::chrono::system_clock::now();
    std::time_t current_time_t = std::chrono::system_clock::to_time_t(current_time);
    irich = std::ctime(&current_time_t);


    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "paziente_id")) {
            strcpy(paziente_id, value);
            read_fields |= 0b1;

        } else if (!strcmp(key, "medico_id")) {
            strcpy(medico_id, value);
            read_fields |= 0b10;

        } else if (!strcmp(key, "amministrativo_id")) {
            strcpy(amministrativo_id, value);
            read_fields |= 0b100;

        } else if (!strcmp(key, "specializzazione_nome")) {
            strcpy(specializzazione_nome, value);
            read_fields |= 0b1000;

        } else if (!strcmp(key, "giornoorariopren")) {
            strcpy(giornoorariopren, value);
            read_fields |= 0b10000;
        }
    }


    if (read_fields != 0b11111) {
        throw std::invalid_argument("Missing fields in RichiestaPrenotazione");
    }

    return new RichiestaPrenotazione( paziente_id, medico_id, amministrativo_id, specializzazione_nome, irich, giornoorariopren);
}
