#include "main.h"
#include <iostream>
#include <cctype>
#include "medico.h"

// Funzione per eseguire la ricerca del medico
void search_medico(redisContext *redConn, DbConnection &db, const char *msg_id, const char *client_id, const char *medico_id) {
    if (!std::all_of(medico_id, medico_id + strlen(medico_id), ::isdigit)) {
        std::cout << "Formato medico_id non valido" << std::endl;
        send_response_status(redConn, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
        return;
    }

    const char *paramValues[1] = { medico_id };
    PGresult *query_res = db.RunPreparedQuery("SELECT * FROM Medico WHERE id = $1", 1, paramValues);

    if (!query_res || PQresultStatus(query_res) != PGRES_TUPLES_OK || PQntuples(query_res) == 0) {
        std::cout << "Errore query o medico non trovato" << std::endl;
        send_response_status(redConn, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
        if (query_res) PQclear(query_res);
        return;
    }

    for (int row = 0; row < PQntuples(query_res); row++) {
        Medico *medico = new Medico(
            PQgetvalue(query_res, row, PQfnumber(query_res, "cf")),
            PQgetvalue(query_res, row, PQfnumber(query_res, "nome")),
            PQgetvalue(query_res, row, PQfnumber(query_res, "cognome")),
            PQgetvalue(query_res, row, PQfnumber(query_res, "nascita"))
        );

        redisReply *redReply = RedisCommand(
            redConn,
            "XADD %s * cf %s nome %s cognome %s nascita %s",
            WRITE_STREAM, medico->cf, medico->nome, medico->cognome, medico->nascita
        );
        assertReplyType(redConn, redReply, REDIS_REPLY_STRING);
        freeReplyObject(redReply);
        delete medico;
    }

    send_response_status(redConn, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, PQntuples(query_res));
    PQclear(query_res);
}

int main() {
    redisContext *redConn;
    redisReply *redReply;
    char query[QUERY_LEN], response[RESPONSE_LEN], msg_id[MESSAGE_ID_LEN], first_key[KEY_LEN], client_id[VALUE_LEN], second_key[KEY_LEN], specializzazione[VALUE_LEN];

    // Connessione a Redis e al database
    redConn = redisConnect(REDIS_SERVER, REDIS_PORT);
    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);

    // Controllo sulla connessione Redis
    if (!redConn || redConn->err) {
        std::cerr << "Errore connessione Redis" << std::endl;
        return -1;
    }

    while (true) {
        redReply = RedisCommand(redConn, "XREADGROUP GROUP main paziente_non_registrato BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);
        
        if (ReadNumStreams(redReply) == 0) {
            if (redReply) freeReplyObject(redReply);
            continue;
        }

        ReadStreamNumMsgID(redReply, 0, 0, msg_id);
        ReadStreamMsgVal(redReply, 0, 0, 0, first_key);
        ReadStreamMsgVal(redReply, 0, 0, 1, client_id);

        if (strcmp(first_key, "client_id")) {
            send_response_status(redConn, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            freeReplyObject(redReply);
            continue;
        }

        ReadStreamMsgVal(redReply, 0, 0, 2, second_key);
        ReadStreamMsgVal(redReply, 0, 0, 3, specializzazione);

        if (strcmp(second_key, "specializzazione") || (ReadStreamMsgNumVal(reply, 0, 0) > 4)){
            send_response_status(redConn, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            freeReplyObject(redReply);
            continue;
        }

        search_medico(redConn, db, msg_id, client_id, medico_id);
        freeReplyObject(redReply);
    }

    redisFree(redConn);
    db.disconnectFromDatabase();
    return 0;
}
