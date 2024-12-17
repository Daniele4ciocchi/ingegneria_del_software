#include "main.h"

int main() {
    redisContext *redConn;
    redisReply *redReply;

    PGresult *queryRes;

    char query[QUERY_LEN], response[RESPONSE_LEN], msg_id[MESSAGE_ID_LEN], first_key[KEY_LEN], client_id[VALUE_LEN], second_key[KEY_LEN], medico_id[VALUE_LEN];

    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    redConn = redisConnect(REDIS_SERVER, REDIS_PORT);

    while(1) {
        // inizio da non modificare 
        redReply = RedisCommand(redConn, "XREADGROUP GROUP main medico BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

        assertReply(redConn, redReply);

        if (ReadNumStreams(redReply) == 0) {
            continue;
        } 

        // Only one stream --> stream_num = 0
        // Only one message in stream --> msg_num = 0
        ReadStreamNumMsgID(redReply, 0, 0, msg_id);

        // Check if the first key/value pair is the client_id
        ReadStreamMsgVal(redReply, 0, 0, 0, first_key);    // Index of first field of msg = 0
        ReadStreamMsgVal(redReply, 0, 0, 1, client_id);    // Index of second field of msg = 1

        if(strcmp(first_key, "client_id")){
            send_response_status(redConn, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        // Take the input
        ReadStreamMsgVal(redReply, 0, 0, 2, second_key);    // Index of third field of msg = 2
        ReadStreamMsgVal(redReply, 0, 0, 3, medico_id);  // Index of fourth field of msg = 3
        
        if(strcmp(second_key, "medico_id") || (ReadStreamMsgNumVal(redReply, 0, 0) > 4)){
            send_response_status(redConn, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }
        // Query per ottenere i pazienti con la medico_id visita
        sprintf(query, "SELECT f.paziente_id AS paziente_id, f.prenotazione_accettata_id AS prenotazione_id, f.ifeed AS ifeed, f.votosodd AS votosodd, f.votopunt AS votopunt FROM feedback AS f, richiestaprenotazione AS rp WHERE f.prenotazione_accettata_id = rp.id AND rp.medico_id = '%s';", medico_id);

        queryRes = db.RunQuery(query, true);
        
        if (PQresultStatus(queryRes) != PGRES_COMMAND_OK && PQresultStatus(queryRes) != PGRES_TUPLES_OK) {
            std::cout << "Errore query o medico non trovato" << std::endl;
            send_response_status(redConn, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        double votosodd = 0;
        double votopunt = 0;

        for(int row = 0; row < PQntuples(queryRes); row++){
            Feedback* feedback;
            feedback = new Feedback(
                                PQgetvalue(queryRes, row, PQfnumber(queryRes, "paziente_id")),
                                PQgetvalue(queryRes, row, PQfnumber(queryRes, "prenotazione_id")),
                                PQgetvalue(queryRes, row, PQfnumber(queryRes, "ifeed")),
                                PQgetvalue(queryRes, row, PQfnumber(queryRes, "votosodd")),
                                PQgetvalue(queryRes, row, PQfnumber(queryRes, "votopunt"))); 

            votosodd += atoi(feedback->votosodd);
            votopunt += atoi(feedback->votopunt);
            delete feedback;
        }
        if (PQntuples(queryRes) != 0) {
            votosodd = votosodd / PQntuples(queryRes);
            votopunt = votopunt / PQntuples(queryRes);
        }
        

        /*
        // nome_medico, cognome_medico, specializzazione, giornoorario, prestazioneavvenuta 
        send_response_status(redConn, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, PQntuples(queryRes));

        redReply = RedisCommand(redConn, "XADD %s media_soddisfazione %s media_puntualità %s ", 
                                 WRITE_STREAM,  votosodd, votopunt);
        assertReplyType(redConn, redReply, REDIS_REPLY_STRING);
        freeReplyObject(redReply);
    
        PQclear(queryRes);
        */

        for(int row = 0; row < 1; row++){


            redReply = RedisCommand(redConn, "XADD %s media_soddisfazione %s media_puntualità %s ", 
                                 WRITE_STREAM,  votosodd, votopunt);
            assertReplyType(redConn, redReply, REDIS_REPLY_STRING);
            freeReplyObject(redReply);

            PQclear(queryRes);
            
        }
         
    }

    db.finish();

    return 0;
}
