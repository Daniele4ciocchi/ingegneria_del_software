#include "main.h"

int main() {
    redisContext *c2r;
    redisReply *reply;

    PGresult *query_res;

    char query[QUERY_LEN], response[RESPONSE_LEN], msg_id[MESSAGE_ID_LEN], first_key[KEY_LEN], client_id[VALUE_LEN], second_key[KEY_LEN], nome_medico[VALUE_LEN];

    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    c2r = redisConnect(REDIS_SERVER, REDIS_PORT);

    while(1) {
        // inizio da non modificare 
        reply = RedisCommand(c2r, "XREADGROUP GROUP main paziente_non_registrato BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

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
        ReadStreamMsgVal(reply, 0, 0, 2, second_key);    // Index of first field of msg = 0
        ReadStreamMsgVal(reply, 0, 0, 3, nome_medico);  // Index of second field of msg = 1
        
        if(strcmp(second_key, "nome_medico") || (ReadStreamMsgNumVal(reply, 0, 0) > 4)){
            send_response_status(c2r, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        std::string str_nome_medico = nome_medico;
        std::string search_parameter = "%"+ str_nome_medico + "%";
        // da rifare la query, bisogna fare il join tra persona e medico per avere il nome del medico
        sprintf(query, "SELECT * FROM medico WHERE name LIKE \'%s\' ", (char*)search_parameter.c_str());

        query_res = db.RunQuery(query, true);

        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        std::list<Medico*> medici;

        for(int row = 0; row < PQntuples(query_res); row++){
            Medico* medico;
            medico = new Medico(PQgetvalue(query_res, row, PQfnumber(query_res, "code")),
                                        PQgetvalue(query_res, row, PQfnumber(query_res, "name")),
                                        PQgetvalue(query_res, row, PQfnumber(query_res, "description")),
                                        PQgetvalue(query_res, row, PQfnumber(query_res, "price")));
            
            medici.push_back(medico);
        }

        send_response_status(c2r, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, PQntuples(query_res));
        
        for(int row = 0; row<PQntuples(query_res); row++){

            Medico *m = medici.front();

            medici.pop_front();

            reply = RedisCommand(c2r, "XADD %s * row %d code %s name %s description %s price %s", WRITE_STREAM, row, p->code, p->name, p->description, p->price);
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);
            freeReplyObject(reply);

        }
    }

    db.finish();

    return 0;
}