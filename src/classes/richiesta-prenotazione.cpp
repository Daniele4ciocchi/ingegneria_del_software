#include "richiesta_prenotazione.h"

RichiestaPrenotazione::RichiestaPrenotazione(
    int id_richiesta,
    int id_paziente,
    int id_medico,
    int id_amministrativo,
    char* specializzazione,
    char* irich_timestamp,
    char* giornoorariopren_timestamp
) {
    id = id_richiesta;
    paziente_id = id_paziente;
    medico_id = id_medico;
    amministrativo_id = id_amministrativo;

    specializzazione_nome = (char*) malloc(sizeof(char) * 101);
    irich = (char*) malloc(sizeof(char) * 20); // Esempio: "2024-11-27 15:30:00"
    giornoorariopren = (char*) malloc(sizeof(char) * 20);

    strcpy(specializzazione_nome, specializzazione);
    strcpy(irich, irich_timestamp);
    strcpy(giornoorariopren, giornoorariopren_timestamp);
}

RichiestaPrenotazione::~RichiestaPrenotazione() {
    free(specializzazione_nome);
    free(irich);
    free(giornoorariopren);
}

RichiestaPrenotazione* RichiestaPrenotazione::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    int id_richiesta;
    int id_paziente;
    int id_medico;
    int id_amministrativo;
    char specializzazione[101];
    char irich[20];
    char giornoorariopren[20];

    char read_fields = 0b0000000;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "id")) {
            id_richiesta = atoi(value);
            read_fields |= 0b0000001;

        } else if (!strcmp(key, "paziente_id")) {
            id_paziente = atoi(value);
            read_fields |= 0b0000010;

        } else if (!strcmp(key, "medico_id")) {
            id_medico = atoi(value);
            read_fields |= 0b0000100;

        } else if (!strcmp(key, "amministrativo_id")) {
            id_amministrativo = atoi(value);
            read_fields |= 0b0001000;

        } else if (!strcmp(key, "specializzazione_nome")) {
            snprintf(specializzazione, 101, "%s", value);
            read_fields |= 0b0010000;

        } else if (!strcmp(key, "irich")) {
            snprintf(irich, 20, "%s", value);
            read_fields |= 0b0100000;

        } else if (!strcmp(key, "giornoorariopren")) {
            snprintf(giornoorariopren, 20, "%s", value);
            read_fields |= 0b1000000;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b1111111) {
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new RichiestaPrenotazione(
        id_richiesta,
        id_paziente,
        id_medico,
        id_amministrativo,
        specializzazione,
        irich,
        giornoorariopren
    );
}
