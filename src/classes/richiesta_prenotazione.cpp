#include "richiesta_prenotazione.h"

RichiestaPrenotazione::RichiestaPrenotazione(char* id, char* paziente_id, char* medico_id, char* amministrativo_id, char* specializzazione_nome, char* giornoorariopren) {
    
    this->id = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->paziente_id = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->medico_id = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->amministrativo_id = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->specializzazione_nome = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->irich = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    this->giornoorariopren = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(this->id, id);
    strcpy(this->paziente_id, paziente_id);
    strcpy(this->medico_id, medico_id);
    strcpy(this->amministrativo_id, amministrativo_id);
    strcpy(this->specializzazione_nome, specializzazione_nome);
    strcpy(this->irich, irich);
    strcpy(this->giornoorariopren, giornoorariopren);

}

RichiestaPrenotazione::~RichiestaPrenotazione() {
    free(this->id);
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


    char id[PARAMETERS_LEN];
    char paziente_id[PARAMETERS_LEN];
    char medico_id[PARAMETERS_LEN];
    char amministrativo_id[PARAMETERS_LEN];
    char specializzazione_nome[PARAMETERS_LEN];
    char irich[PARAMETERS_LEN];
    char giornoorariopren[PARAMETERS_LEN];

    char read_fields = 0b00;


    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "id")) {
            strcpy(id, value);
            read_fields |= 0b01;

        } else if (!strcmp(key, "paziente_id")) {
            strcpy(paziente_id, value);
            read_fields |= 0b10;

        } else if (!strcmp(key, "medico_id")) {
            strcpy(medico_id, value);
            read_fields |= 0b100;

        } else if (!strcmp(key, "amministrativo_id")) {
            strcpy(amministrativo_id, value);
            read_fields |= 0b1000;

        } else if (!strcmp(key, "specializzazione_nome")) {
            strcpy(specializzazione_nome, value);
            read_fields |= 0b10000;

        } else if (!strcmp(key, "irich")) {
            strcpy(irich, value);
            read_fields |= 0b1000000;

        } else if (!strcmp(key, "giornoorariopren")) {
            strcpy(giornoorariopren, value);
            read_fields |= 0b100000;
        }
    }

    if (read_fields != 0b111111) {
        throw std::invalid_argument("Missing fields in RichiestaPrenotazione");
    }

    return new RichiestaPrenotazione(id, paziente_id, medico_id, amministrativo_id, specializzazione_nome, giornoorariopren);
}

std::string RichiestaPrenotazione::to_insert_query() const {
    std::ostringstream query;
    query << "INSERT INTO richiestaprenotazione (id, paziente_id, medico_id, amministrativo_id, specializzazione_nome,irich, giornoorariopren) VALUES ("
          << "'" << id << "', "
          << "'" << paziente_id << "', "
          << "'" << medico_id << "', "
          << "'" << amministrativo_id << "', "
          << "'" << specializzazione_nome << "', "
          << "'" << irich << "', "
          << "'" << giornoorariopren << "');";
    return query.str();
}

std::string RichiestaPrenotazione::to_update_query() const {
    std::ostringstream query;
    query << "UPDATE prenotazioni SET "
          << "paziente_id = '" << paziente_id << "', "
          << "medico_id = '" << medico_id << "', "
          << "amministrativo_id = '" << amministrativo_id << "', "
          << "specializzazione_nome = '" << specializzazione_nome << "', "
          << "giornoorariopren = '" << giornoorariopren << "' "
          << "irich = '" << irich << "' "
          << "WHERE id = '" << id << "';";
    return query.str();
}

