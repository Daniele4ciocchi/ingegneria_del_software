#include "main.h"

int main() {
    redisContext *redConn;
    redisReply *redReply;

    PGresult *queryRes;

    char query[QUERY_LEN], response[RESPONSE_LEN], msg_id[MESSAGE_ID_LEN], first_key[KEY_LEN], client_id[VALUE_LEN], second_key[KEY_LEN], specializzazione[VALUE_LEN];

    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    redConn = redisConnect(REDIS_SERVER, REDIS_PORT);

    while(1) {
        // inizio da non modificare 
        redReply = RedisCommand(redConn, "XREADGROUP GROUP main paziente_non_registrato BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

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
        ReadStreamMsgVal(redReply, 0, 0, 3, specializzazione);  // Index of fourth field of msg = 3
        
        if(strcmp(second_key, "specializzazione") || (ReadStreamMsgNumVal(redReply, 0, 0) > 4)){
            send_response_status(redConn, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        std::string str_specializzazione = specializzazione;
        std::string search_parameter = "%" + str_specializzazione + "%";

        // Query per ottenere i medici con la specializzazione richiesta
        sprintf(query, "SELECT p.cf, p.nome, p.cognome, p.nascita FROM medico m, persona p, medico_specializzazione s WHERE p.cf = m.cf AND m.id = s.medico_id AND s.specializzazione_nome = '%s';", specializzazione);

        queryRes = db.RunQuery(query, true);
        
        if (PQresultStatus(queryRes) != PGRES_COMMAND_OK && PQresultStatus(queryRes) != PGRES_TUPLES_OK) {
            std::cout << "Errore query o medico non trovato" << std::endl;
            send_response_status(redConn, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        print_queryResult(queryRes, specializzazione);

        std::list<Persona*> medici;

        for(int row = 0; row < PQntuples(queryRes); row++){
            Persona* medico;
            medico = new Persona(PQgetvalue(queryRes, row, PQfnumber(queryRes, "cf")),
                                PQgetvalue(queryRes, row, PQfnumber(queryRes, "nome")),
                                PQgetvalue(queryRes, row, PQfnumber(queryRes, "cognome")),
                                PQgetvalue(queryRes, row, PQfnumber(queryRes, "nascita")));
            
            medici.push_back(medico);
        }
   
        send_response_status(redConn, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, PQntuples(queryRes));
        
        for(int row = 0; row < PQntuples(queryRes); row++){

            Persona *m = medici.front();

            medici.pop_front();

            redReply = RedisCommand(redConn, "XADD %s * row %d nome %s cognome %s", 
                                 WRITE_STREAM, row,  m->nome, m->cognome);
            cout << redReply->str << endl;
            assertReplyType(redConn, redReply, REDIS_REPLY_STRING);
            freeReplyObject(redReply);

        }
       

    }

    db.finish();

    return 0;
}
