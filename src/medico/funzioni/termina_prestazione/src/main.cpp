#include "main.h"

int main()
{
    redisContext *redConn;
    redisReply *redReply;

    PGresult *queryRes;

    char query[QUERY_LEN], response[RESPONSE_LEN], msg_id[MESSAGE_ID_LEN], first_key[KEY_LEN], client_id[VALUE_LEN], second_key[KEY_LEN], prenotazione_id[KEY_LEN];

    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    redConn = redisConnect(REDIS_SERVER, REDIS_PORT);

    while (1)
    {
        redReply = RedisCommand(redConn, "XREADGROUP GROUP main medico BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

        assertReply(redConn, redReply);

        if (ReadNumStreams(redReply) == 0)
        {
            continue;
        }

        ReadStreamNumMsgID(redReply, 0, 0, msg_id);

        ReadStreamMsgVal(redReply, 0, 0, 0, first_key);
        ReadStreamMsgVal(redReply, 0, 0, 1, client_id);

        if (strcmp(first_key, "client_id"))
        {
            send_response_status(redConn, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        // Take the input
        ReadStreamMsgVal(redReply, 0, 0, 2, second_key);
        ReadStreamMsgVal(redReply, 0, 0, 3, prenotazione_id);

        if (strcmp(second_key, "prenotazione_id") || (ReadStreamMsgNumVal(redReply, 0, 0) > 4))
        {
            send_response_status(redConn, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        sprintf(query, "UPDATE prenotazioneaccettata SET prestazioneavvenuta = true WHERE richiesta_id = \'%s\'", prenotazione_id);

        queryRes = db.RunQuery(query, false);

        if (PQresultStatus(queryRes) != PGRES_COMMAND_OK && PQresultStatus(queryRes) != PGRES_TUPLES_OK)
        {
            send_response_status(redConn, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        send_response_status(redConn, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, 0);
    }

    db.finish();

    return 0;
}
