#include "main.h"

int main() {
    redisContext *redConn;
    redisReply *redReply;

    PGresult *queryRes;

    char query[QUERY_LEN], response[RESPONSE_LEN], msg_id[MESSAGE_ID_LEN], first_key[KEY_LEN], client_id[VALUE_LEN], second_key[KEY_LEN], medico_id[VALUE_LEN];

    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    redConn = redisConnect(REDIS_SERVER, REDIS_PORT);

    if (redConn == NULL || redConn->err) {
        if (redConn) {
            std::cerr << "Redis connection error: " << redConn->errstr << std::endl;
        } else {
            std::cerr << "Redis connection allocation error." << std::endl;
        }
        return EXIT_FAILURE;
    }

    while(1) {
        // inizio da non modificare 
        redReply = RedisCommand(redConn, "XREADGROUP GROUP main medico BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

        if (!redReply) {
            std::cerr << "Errore durante XREADGROUP." << std::endl;
            continue;
        }
        assertReply(redConn, redReply);

        if (ReadNumStreams(redReply) == 0) {
            freeReplyObject(redReply);
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
            freeReplyObject(redReply);
            continue;
        }

        // Take the input
        ReadStreamMsgVal(redReply, 0, 0, 2, second_key);    // Index of third field of msg = 2
        ReadStreamMsgVal(redReply, 0, 0, 3, medico_id);  // Index of fourth field of msg = 3
        
        if(strcmp(second_key, "medico_id") || (ReadStreamMsgNumVal(redReply, 0, 0) > 4)){
            send_response_status(redConn, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            freeReplyObject(redReply);
            continue;
        }
        
        freeReplyObject(redReply);
    
        // Query per ottenere i pazienti con la medico_id visita
        sprintf(query, "SELECT f.votosodd AS votosodd, f.votopunt AS votopunt FROM feedback AS f, richiestaprenotazione AS rp WHERE f.prenotazione_accettata_id = rp.id AND rp.medico_id = '%s';", medico_id);

        queryRes = db.RunQuery(query, true);
        
        if (!queryRes || (PQresultStatus(queryRes) != PGRES_TUPLES_OK)) {
            std::cerr << "Errore durante l'esecuzione della query o medico non trovato." << std::endl;
            send_response_status(redConn, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            if (queryRes) PQclear(queryRes);
            continue;
        }

        double votosodd = 0;
        double votopunt = 0;

        int num_rows = PQntuples(queryRes);
        for (int row = 0; row < num_rows; row++) {
            votosodd += atoi(PQgetvalue(queryRes, row, PQfnumber(queryRes, "votosodd")));
            votopunt += atoi(PQgetvalue(queryRes, row, PQfnumber(queryRes, "votopunt")));
        }


        if (num_rows > 0) {
            votosodd /= num_rows;
            votopunt /= num_rows;
        }

        std::string votosodd_str = std::to_string(votosodd);
        std::string votopunt_str = std::to_string(votopunt);

        int row = 0;
         
        redReply = RedisCommand(redConn, "XADD %s * row %d media_soddisfazione %.2f media_puntualita %.2f", 
                                WRITE_STREAM, row, votosodd_str, votopunt_str);
        
        send_response_status(redConn, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id,0);

        assertReplyType(redConn, redReply, REDIS_REPLY_STRING);
        freeReplyObject(redReply);
        
            
        }

    db.finish();

    return 0;
}
