#include "main.h"

int main() {
    redisContext *c2r;
    redisReply *reply;

    PGresult *query_res;

    char query[QUERY_LEN], response[RESPONSE_LEN], msg_id[MESSAGE_ID_LEN], first_key[KEY_LEN], client_id[VALUE_LEN], second_key[KEY_LEN], prenotazione_id[KEY_LEN];

    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    c2r = redisConnect(REDIS_SERVER, REDIS_PORT);


    while(1) {

        reply = RedisCommand(c2r, "XREADGROUP GROUP main medico BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

        assertReply(c2r, reply);

        if (ReadNumStreams(reply) == 0) {
            continue;
        }

        // Only one stream --> stream_num = 0
        // Only one message in stream --> msg_num = 0
        ReadStreamNumMsgID(reply, 0, 0, msg_id);

        // Check if the first key/value pair is the client_id
        ReadStreamMsgVal(reply, 0, 0, 0, first_key);    // Index of first field of msg = 0
        ReadStreamMsgVal(reply, 0, 0, 1, client_id);    // Index of second field of msg = 1

        if(strcmp(first_key, "client_id")){
            send_response_status(c2r, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        // Take the input
        ReadStreamMsgVal(reply, 0, 0, 2, second_key);    // Index of third field of msg = 2
        ReadStreamMsgVal(reply, 0, 0, 3, prenotazione_id);  // Index of fourth field of msg = 3
        
        if(strcmp(second_key, "prenotazione_id") || (ReadStreamMsgNumVal(reply, 0, 0) > 4)){
            send_response_status(c2r, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        sprintf(query, "UPDATE prenotazioneaccettata SET prestazioneavvenuta = true WHERE richiesta_id = \'%s\'", prenotazione_id);
        
        query_res = db.RunQuery(query, false);

        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        send_response_status(c2r, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, 0);

    }

    db.finish();

    return 0;
}


