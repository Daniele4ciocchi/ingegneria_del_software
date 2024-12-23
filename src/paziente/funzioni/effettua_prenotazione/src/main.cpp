#include "main.h"

int main()
{
    redisContext *redConn;
    redisReply *redReply;

    PGresult *queryRes;

    char query[QUERY_LEN], response[RESPONSE_LEN], msg_id[MESSAGE_ID_LEN], first_key[KEY_LEN], client_id[VALUE_LEN];

    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    redConn = redisConnect(REDIS_SERVER, REDIS_PORT);

    RichiestaPrenotazione *richiestaPrenotazione;

    while (1)
    {

        redReply = RedisCommand(redConn, "XREADGROUP GROUP main paziente BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

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
            richiestaPrenotazione = RichiestaPrenotazione::from_stream(redReply, 0, 0);
        }
        catch (std::invalid_argument exp)
        {
            send_response_status(redConn, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        sprintf(query, "INSERT INTO richiestaprenotazione (paziente_id, medico_id, amministrativo_id, specializzazione_nome, irich, giornoorariopren) VALUES (\'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\')",
                richiestaPrenotazione->paziente_id, richiestaPrenotazione->medico_id, richiestaPrenotazione->amministrativo_id, richiestaPrenotazione->specializzazione_nome, richiestaPrenotazione->irich, richiestaPrenotazione->giornoorariopren);

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