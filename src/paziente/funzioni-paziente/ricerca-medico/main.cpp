#include "main.h"
#include <iostream>
#include "medico.h"

int main() {
    redisContext *redConn;
    redisReply *redReply;
    PGresult *query_res;

    char query[QUERYSIZE], msg_id[MSGIDSIZE], first_key[KEYSIZE], client_id[VALUESIZE], second_key[KEYSIZE], medico_id[PRMTRSIZE];

    DbConnection db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    redConn = redisConnect(REDIS_SERVER, REDIS_PORT);

    while(true) {
        redReply = RedisCommand(redConn, "XREADGROUP GROUP main customer BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

        assertReply(redConn, redReply);

        if (ReadNumStreams(redReply) == 0) {
            continue;
        }

        ReadStreamNumMsgID(redReply, 0, 0, msg_id);

        ReadStreamMsgVal(redReply, 0, 0, 0, first_key);
        ReadStreamMsgVal(redReply, 0, 0, 1, client_id);

        if(strcmp(first_key, "client_id")){
            std::cout << "Errore client id" << std::endl;
            send_response_status(redConn, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        ReadStreamMsgVal(redReply, 0, 0, 2, second_key);
        ReadStreamMsgVal(redReply, 0, 0, 3, medico_id);

        if(strcmp(second_key, "medico_id") || (ReadStreamMsgNumVal(redReply, 0, 0) > 4)){
            std::cout << "Errore medico id" << std::endl;
            send_response_status(redConn, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }
        std::cout << "medico id:" << medico_id << std::endl;

        sprintf(query, "SELECT * FROM Medico WHERE id = %s", medico_id);

        std::cout << "ecco la query: " << query << std::endl;

        query_res = db.RunQuery(query, true);
        std::cout << "Numero di righe in output : " << PQntuples(query_res) << std::endl;
        if ((PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) || (PQntuples(query_res) == 0)) {
            std::cout << "Errore query" << std::endl;
            send_response_status(redConn, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        for(int row = 0; row < PQntuples(query_res); row++) {
            Medico *medico;
            medico = new Medico(PQgetvalue(query_res, row, PQfnumber(query_res, "cf")),
                                PQgetvalue(query_res, row, PQfnumber(query_res, "nome")),
                                PQgetvalue(query_res, row, PQfnumber(query_res, "cognome")),
                                PQgetvalue(query_res, row, PQfnumber(query_res, "nascita")));

            redReply = RedisCommand(redConn, "XADD %s * cf %s nome %s cognome %s nascita %s",
                                    WRITE_STREAM, medico->cf, medico->nome, medico->cognome, medico->nascita);
            assertReplyType(redConn, redReply, REDIS_REPLY_STRING);
            freeReplyObject(redReply);

            delete medico;
        }

        send_response_status(redConn, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, PQntuples(query_res));
    }

    db.disconnectFromDatabase();

    return 0;
}

void send_response_status(redisContext *redConn, const char *stream, const char *client_id, const char *status, const char *msg_id, int num_results) {
    redisReply *redReply;
    redReply = RedisCommand(redConn, "XADD %s * client_id %s status %s msg_id %s num_results %d", stream, client_id, status, msg_id, num_results);
    assertReplyType(redConn, redReply, REDIS_REPLY_STRING);
    freeReplyObject(redReply);
}