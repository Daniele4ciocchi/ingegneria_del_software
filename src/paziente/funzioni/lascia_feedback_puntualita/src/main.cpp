#include "main.h"

int main() {
    redisContext *c2r;
    redisReply *reply;

    PGresult *query_res;

    char query[QUERY_LEN], response[RESPONSE_LEN], msg_id[MESSAGE_ID_LEN], first_key[KEY_LEN], client_id[VALUE_LEN];

    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    c2r = redisConnect(REDIS_SERVER, REDIS_PORT);

    if (c2r == NULL || c2r->err) {
        if (c2r) {
            printf("Error: %s\n", c2r->errstr);
        } else {
            printf("Can't allocate redis context\n");
        }
        return 1;
    }

    Feedback* feedback;

    while(1) {
        reply = RedisCommand(c2r, "XREADGROUP GROUP main customer BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

        assertReply(c2r, reply);

        if (ReadNumStreams(reply) == 0) {
            freeReplyObject(reply);
            continue;
        }

        // Only one stream --> stream_num = 0
        // Only one message in stream --> msg_num = 0
        ReadStreamNumMsgID(reply, 0, 0, msg_id);

        // Check if the first key/value pair is the client_id
        ReadStreamMsgVal(reply, 0, 0, 0, first_key);    // Index of first field of msg = 0
        ReadStreamMsgVal(reply, 0, 0, 1, client_id);    // Index of second field of msg = 1

        if(strcmp(first_key, "client_id") != 0){
            send_response_status(c2r, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            freeReplyObject(reply);
            continue;
        }

        // Convert request
        try {
            feedback = Feedback::from_stream(reply, 0, 0);
        } catch (std::invalid_argument& exp) {
            send_response_status(c2r, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            freeReplyObject(reply);
            continue;
        }

        sprintf(query, "INSERT INTO feedback (paziente_id, prenotazione_accettata_id, ifeed, votosodd, votopunt) "
                       "VALUES (%d, %d, '%s', %d, %d) RETURNING id;",
                       feedback->paziente_id, feedback->prenotazione_accettata_id, feedback->ifeed, feedback->votosodd, feedback->votopunt);

        query_res = db.RunQuery(query, false);

        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            PQclear(query_res);
            freeReplyObject(reply);
            continue;
        }

        send_response_status(c2r, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, 0);

        PQclear(query_res);
        freeReplyObject(reply);
    }

    db.finish();
    redisFree(c2r);

    return 0;
}