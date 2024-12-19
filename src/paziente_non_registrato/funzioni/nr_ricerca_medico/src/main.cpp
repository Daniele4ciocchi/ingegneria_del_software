#include "main.h"

int main() {
    redisContext *redConn;
    redisReply *redReply;

    PGresult *queryRes;

    char query[QUERY_LEN], response[RESPONSE_LEN], msg_id[MESSAGE_ID_LEN], first_key[KEY_LEN], client_id[VALUE_LEN], second_key[KEY_LEN], specializzazione[VALUE_LEN];

    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    redConn = redisConnect(REDIS_SERVER, REDIS_PORT);

    while(1) {
        redReply = RedisCommand(redConn, "XREADGROUP GROUP main paziente_non_registrato BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

        assertReply(redConn, redReply);

        if (ReadNumStreams(redReply) == 0) {
            continue;
        } 

        
        ReadStreamNumMsgID(redReply, 0, 0, msg_id);

        
        ReadStreamMsgVal(redReply, 0, 0, 0, first_key);
        ReadStreamMsgVal(redReply, 0, 0, 1, client_id);

        if(strcmp(first_key, "client_id")){
            send_response_status(redConn, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        
        ReadStreamMsgVal(redReply, 0, 0, 2, second_key);    
        ReadStreamMsgVal(redReply, 0, 0, 3, specializzazione);  
        
        if(strcmp(second_key, "specializzazione") || (ReadStreamMsgNumVal(redReply, 0, 0) > 4)){
            send_response_status(redConn, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        sprintf(query, "SELECT p.cf, p.nome, p.cognome, p.nascita FROM medico m, persona p, medico_specializzazione s WHERE p.cf = m.cf AND m.id = s.medico_id AND s.specializzazione_nome = '%s';", specializzazione);

        queryRes = db.RunQuery(query, true);
        
        if (PQresultStatus(queryRes) != PGRES_COMMAND_OK && PQresultStatus(queryRes) != PGRES_TUPLES_OK) {
            std::cout << "Errore query o medico non trovato" << std::endl;
            send_response_status(redConn, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

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

        redReply = RedisCommand(redConn, "XADD %s * row %d ", WRITE_STREAM, 0);

        assertReplyType(redConn, redReply, REDIS_REPLY_STRING);
        freeReplyObject(redReply);
        
        
        for(int row = 0; row < PQntuples(queryRes); row++){

            Persona *m = medici.front();

            medici.pop_front();

            redReply = RedisCommand(redConn, "XADD %s * row %d nome %s cognome %s", 
                                 WRITE_STREAM, row,  m->nome, m->cognome);
            assertReplyType(redConn, redReply, REDIS_REPLY_STRING);
            freeReplyObject(redReply);

        }
       

    }

    db.finish();

    return 0;
}
