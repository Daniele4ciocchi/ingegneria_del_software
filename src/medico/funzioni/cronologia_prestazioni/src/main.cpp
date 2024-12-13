#include "main.h"
#include <sstream>

std::string get_prestazioni_history(Con2DB &db, const std::string &medico_id) {
    std::ostringstream query;
    query << "SELECT id, paziente_id, data_ora, durata, descrizione FROM prestazioni "
          << "WHERE medico_id = '" << medico_id << "' ORDER BY data_ora DESC;";

    PGresult *query_res = db.RunQuery((char *)query.str().c_str(), false);
    if (PQresultStatus(query_res) != PGRES_TUPLES_OK) {
        PQclear(query_res);
        throw std::runtime_error("Errore nella query per la cronologia delle prestazioni.");
    }

    // Comporre una stringa con i dati della cronologia
    std::ostringstream result_stream;
    int rows = PQntuples(query_res);
    for (int i = 0; i < rows; i++) {
        result_stream << "id=" << PQgetvalue(query_res, i, 0)
                      << ";paziente_id=" << PQgetvalue(query_res, i, 1)
                      << ";data_ora=" << PQgetvalue(query_res, i, 2)
                      << ";durata=" << PQgetvalue(query_res, i, 3)
                      << ";descrizione=" << PQgetvalue(query_res, i, 4);
        if (i < rows - 1) result_stream << "|";
    }
    PQclear(query_res);

    return result_stream.str();
}

int main() {
    redisContext *redConn;
    redisReply *redReply;

    char msg_id[MESSAGE_ID_LEN], first_key[KEY_LEN], medico_id[VALUE_LEN];

    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    redConn = redisConnect(REDIS_SERVER, REDIS_PORT);
    if (redConn == nullptr || redConn->err) {
        std::cerr << "Errore nella connessione a Redis: " << redConn->errstr << std::endl;
        return -1;
    }

    while (true) {
        // Leggere un messaggio dalla coda Redis
        redReply = RedisCommand(redConn, "XREADGROUP GROUP main prestazioni_history BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);
        assertReply(redConn, redReply);

        if (ReadNumStreams(redReply) == 0) {
            freeReplyObject(redReply);
            continue;
        }

        // Estrazione del messaggio
        ReadStreamNumMsgID(redReply, 0, 0, msg_id);
        ReadStreamMsgVal(redReply, 0, 0, 0, first_key);
        ReadStreamMsgVal(redReply, 0, 0, 1, medico_id);

        if (strcmp(first_key, "medico_id") != 0) {
            send_response_status(redConn, WRITE_STREAM, medico_id, "BAD_REQUEST", msg_id, 0);
            freeReplyObject(redReply);
            continue;
        }

        // Recupero cronologia delle prestazioni
        try {
            std::string history = get_prestazioni_history(db, medico_id);

            // Aggiungere la cronologia al flusso Redis
            RedisCommand(redConn, "XADD %s * medico_id %s history %s", WRITE_STREAM, medico_id, history.c_str());
            send_response_status(redConn, WRITE_STREAM, medico_id, "REQUEST_SUCCESS", msg_id, 1);
        } catch (const std::exception &e) {
            std::cerr << "Errore: " << e.what() << std::endl;
            send_response_status(redConn, WRITE_STREAM, medico_id, "DB_ERROR", msg_id, 0);
        }

        freeReplyObject(redReply);
    }

    db.finish();
    redisFree(redConn);
    return 0;
}
