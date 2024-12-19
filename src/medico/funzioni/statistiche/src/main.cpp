#include "main.h"

int main()
{
    redisContext *redConn;
    redisReply *redReply;

    PGresult *queryRes;

    char query[QUERY_LEN], response[RESPONSE_LEN], msg_id[MESSAGE_ID_LEN], first_key[KEY_LEN], client_id[VALUE_LEN], second_key[KEY_LEN], medico_id[VALUE_LEN];

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

        ReadStreamMsgVal(redReply, 0, 0, 2, second_key);
        ReadStreamMsgVal(redReply, 0, 0, 3, medico_id);

        if (strcmp(second_key, "medico_id") || (ReadStreamMsgNumVal(redReply, 0, 0) > 4))
        {
            send_response_status(redConn, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        sprintf(query, "SELECT AVG(f.votosodd) AS media_voto_soddisfazione, AVG(f.votopunt) AS media_voto_puntualita FROM richiestaprenotazione rp JOIN prenotazioneaccettata pa ON rp.id = pa.richiesta_id JOIN feedback f ON pa.richiesta_id = f.prenotazione_accettata_id WHERE rp.medico_id = '%s' GROUP BY rp.medico_id;", medico_id);

        queryRes = db.RunQuery(query, true);

        if (!queryRes || (PQresultStatus(queryRes) != PGRES_TUPLES_OK))
        {
            send_response_status(redConn, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        char *votosodd = NULL;
        char *votopunt = NULL;

        votosodd = PQgetvalue(queryRes, 0, PQfnumber(queryRes, "media_voto_soddisfazione"));
        votopunt = PQgetvalue(queryRes, 0, PQfnumber(queryRes, "media_voto_puntualita"));

        send_response_status(redConn, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, 2);

        redReply = RedisCommand(redConn, "XADD %s * row %d ",
                                WRITE_STREAM, 0);

        assertReplyType(redConn, redReply, REDIS_REPLY_STRING);
        freeReplyObject(redReply);

        redReply = RedisCommand(redConn, "XADD %s * row %d media_soddisfazione %s media_puntualita %s",
                                WRITE_STREAM, 1, votosodd, votopunt);

        assertReplyType(redConn, redReply, REDIS_REPLY_STRING);
        freeReplyObject(redReply);
    }

    db.finish();

    return 0;
}
