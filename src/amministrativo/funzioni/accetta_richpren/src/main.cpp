#include "main.h"

int main() {
    redisContext *c2r;
    redisReply *reply;

    PGresult *query_res;

    char query[QUERY_LEN], response[RESPONSE_LEN], msg_id[MESSAGE_ID_LEN], first_key[KEY_LEN], client_id[VALUE_LEN];

    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    c2r = redisConnect(REDIS_SERVER, REDIS_PORT);

    PrenotazioneAccettata* prenotazioneAccettata;

    while(1) {

        reply = RedisCommand(c2r, "XREADGROUP GROUP main amministrativo BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

        assertReply(c2r, reply);

        if (ReadNumStreams(reply) == 0) {
            continue;
        }

        ReadStreamNumMsgID(reply, 0, 0, msg_id);

        ReadStreamMsgVal(reply, 0, 0, 0, first_key);    
        ReadStreamMsgVal(reply, 0, 0, 1, client_id);    

        if(strcmp(first_key, "client_id")){
            send_response_status(c2r, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        try{
            prenotazioneAccettata = PrenotazioneAccettata::from_stream(reply, 0, 0);
        }
        catch(std::invalid_argument exp){
            send_response_status(c2r, WRITE_STREAM, client_id, "BAD_REQUESTTT", msg_id, 0);
            continue;
        }

        sprintf(query, "INSERT INTO prenotazioneaccettata (richiesta_id, iaccet, prestazioneavvenuta) VALUES (\'%s\', \'%s\', \'%s\');", 
                        prenotazioneAccettata->richiesta_id, prenotazioneAccettata->iaccet, prenotazioneAccettata->prestazioneavvenuta);


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