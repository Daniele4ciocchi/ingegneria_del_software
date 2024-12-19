#include "main.h"

int main()
{
    redisContext *redConn;
    redisReply *redReply;

    PGresult *queryRes;

    char query[QUERY_LEN], response[RESPONSE_LEN], msg_id[MESSAGE_ID_LEN], first_key[KEY_LEN], client_id[VALUE_LEN], second_key[KEY_LEN], amministrativo_id[VALUE_LEN];

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
        ReadStreamMsgVal(redReply, 0, 0, 3, amministrativo_id);

        if (strcmp(second_key, "amministrativo_id") || (ReadStreamMsgNumVal(redReply, 0, 0) > 4))
        {
            send_response_status(redConn, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        sprintf(query, "SELECT rp.paziente_id AS paziente_id, rp.medico_id AS medico_id, rp.specializzazione_nome AS specializzazione_visita, rp.giornoorariopren AS giorno_visita, rp.irich AS irich, pa.prestazioneavvenuta AS visita_avvenuta FROM prenotazioneaccettata pa, richiestaprenotazione rp WHERE pa.richiesta_id = rp.id AND rp.amministrativo_id = '%s';", amministrativo_id);

        queryRes = db.RunQuery(query, true);

        if (PQresultStatus(queryRes) != PGRES_COMMAND_OK && PQresultStatus(queryRes) != PGRES_TUPLES_OK)
        {
            std::cout << "Errore query o medico non trovato" << std::endl;
            send_response_status(redConn, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        std::list<RichiestaPrenotazione *> visite;

        for (int row = 0; row < PQntuples(queryRes); row++)
        {
            RichiestaPrenotazione *visita;
            visita = new RichiestaPrenotazione(
                PQgetvalue(queryRes, row, PQfnumber(queryRes, "paziente_id")),
                PQgetvalue(queryRes, row, PQfnumber(queryRes, "medico_id")),
                PQgetvalue(queryRes, row, PQfnumber(queryRes, "specializzazione_visita")),
                PQgetvalue(queryRes, row, PQfnumber(queryRes, "giorno_visita")),
                PQgetvalue(queryRes, row, PQfnumber(queryRes, "irich")),
                PQgetvalue(queryRes, row, PQfnumber(queryRes, "visita_avvenuta")));
            visite.push_back(visita);
        }

        send_response_status(redConn, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, PQntuples(queryRes));

        redReply = RedisCommand(redConn, "XADD %s * row %d ", WRITE_STREAM, 0);

        assertReplyType(redConn, redReply, REDIS_REPLY_STRING);
        freeReplyObject(redReply);

        for (int row = 0; row < PQntuples(queryRes); row++)
        {

            RichiestaPrenotazione *v = visite.front();

            visite.pop_front();

            redReply = RedisCommand(redConn, "XADD %s * row %d paziente_id %s medico_id %s specializzazione %s giorno %s prestazioneavvenuta %s",
                                    WRITE_STREAM, row, v->paziente_id, v->medico_id, v->amministrativo_id, v->specializzazione_nome, v->giornoorariopren);
            assertReplyType(redConn, redReply, REDIS_REPLY_STRING);
            freeReplyObject(redReply);
        }
    }

    db.finish();

    return 0;
}
