#include "main.h"

int main()
{
    redisContext *redConn;
    redisReply *redReply;

    PGresult *queryRes;

    char query[QUERY_LEN], response[RESPONSE_LEN], msg_id[MESSAGE_ID_LEN], first_key[KEY_LEN], client_id[VALUE_LEN];

    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    redConn = redisConnect(REDIS_SERVER, REDIS_PORT);

    PrenotazioneAccettata *prenotazioneAccettata;

    while (1)
    {

        redReply = RedisCommand(redConn, "XREADGROUP GROUP main amministrativo BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

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

        try
        {
            prenotazioneAccettata = PrenotazioneAccettata::from_stream(redReply, 0, 0);
        }
        catch (std::invalid_argument exp)
        {
            send_response_status(redConn, WRITE_STREAM, client_id, "BAD_REQUESTTT", msg_id, 0);
            continue;
        }

        sprintf(query, "INSERT INTO prenotazioneaccettata (richiesta_id, iaccet, prestazioneavvenuta) VALUES (\'%s\', \'%s\', \'%s\');",
                prenotazioneAccettata->richiesta_id, prenotazioneAccettata->iaccet, prenotazioneAccettata->prestazioneavvenuta);

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