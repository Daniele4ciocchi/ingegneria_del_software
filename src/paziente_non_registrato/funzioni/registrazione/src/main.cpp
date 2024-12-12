#include "main.h"

int main() {
    redisContext *c2r;
    redisReply *reply;

    PGresult *query_res, *query_res_2;

    char query[QUERY_LEN], query_2[QUERY_LEN], response[RESPONSE_LEN], msg_id[MESSAGE_ID_LEN], first_key[KEY_LEN], client_id[VALUE_LEN];

    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    c2r = redisConnect(REDIS_SERVER, REDIS_PORT);

    Persona* persona;
    Paziente* paziente;

    while(1) {

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
            send_response_status(c2r, WRITE_STREAM, client_id, "BAD_REQUESTTTT", msg_id, 0);
            continue;
        }

        // Convert request
        try{
            // campi per lo stream redis 
            // 0: cf, 1: nome, 2: cognome, 3: nascita, 4: indirizzo, 5: email, 6 telefono
            persona = Persona::from_stream(reply, 0, 0); // passo prima lo stream redis a persona e poi a paziente
            paziente = Paziente::from_stream(reply, 0, 0);
        }

        catch(std::invalid_argument exp){
            send_response_status(c2r, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        //prima query

        sprintf(query, "INSERT INTO Persona (cf, nome, cognome, nascita) VALUES (\'%s\', \'%s\', \'%s\', \'%s\')", 
                        persona->cf, persona->nome, persona->cognome, persona->nascita);

        query_res = db.RunQuery(query, false);
        
        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        send_response_status(c2r, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, 0);

        //seconda query
        
        sprintf(query_2, "INSERT INTO Paziente (cf, indirizzo, email, telefono) VALUES (\'%s\', \'%s\', \'%s\', \'%s\')", 
                        paziente->cf, paziente->indirizzo, paziente->email, paziente->telefono);
        
        query_res_2 = db.RunQuery(query, false);
        
        if (PQresultStatus(query_res_2) != PGRES_COMMAND_OK && PQresultStatus(query_res_2) != PGRES_TUPLES_OK) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        send_response_status(c2r, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, 0);

    }

    db.finish();

    return 0;
}