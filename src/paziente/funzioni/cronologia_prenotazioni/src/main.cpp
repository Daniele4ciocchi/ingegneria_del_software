#include "main.h"
#include <sstrem>

//ottengo la cronologia

std::string get_booking_history(Con2DB &db, const std::string &client_id) {
    std::ostringstream query;
    query << "SELECT id, medico_id, data_ora, durata FROM prenotazioni" <<"WHERE client_id = '" << client_id << "'ORDER BY data_ora DESC;";

    PGresult *query_res = db.RunQuery((char*)query.str().c_str(),false);
    if (PQresultStatus(query_res) != PGRES_TUPLES_OK){
        PQclear(query_res);
        throw std::runtime_error("Errore nella query per la cronologia.");

    }
    //stringa con i dati
    std::ostringstream result_stream;
    int rows = PQntuples(query_res);
    for (int i = 0; i < rows; i++) {
        result_stream << "id=" <<PQgetvalue(query_res, i, 0)
                    << ";medico_id=" <<PQgetvalue(query_res, i, 1)
                    << ";data_ora=" <<PQgetvalue(queryres,i, 2)
                    << ";durata=" <<PQgetvalue(queryres,i, 3)
        if (i<rows-1)result_stream << "|";
    }
    PQclear(query_res);

    return result_stream.str();
}

int main(){
    redisContext *c2r;
    redisReply *reply;

    char msg_id[MESSAGE_ID_LEN], first_key[KEY_LEN], client_id[VAUE_LEN];

    //connessione al db di Redis
    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    c2r= redisConnect(REDIS_SERVER,REDIS_PORT);
    if (c2r == nullptr||c2r->err){
        std::cerr<<"Errore nella connessione a Redis" << c2r -> errstr <<std::endl;
        return -1;
    }

    while (true) {
        //legge un messaggio dalla coda redis
        reply = RedisCommand(c2r "XREADGROUP main booking_history BLOCK 0 COUNT 1 STREAMS %s >",READ_STREAM);
        if (!reply ||reply->type == REDIS_REPLY_ERROR){
            if(reply) freeReplyObject(reply);
            continue;
        }
        if (ReadNumStreams(reply) == 0)
        {
            freeReplyObject(reply);
            continue;
        }
        //Estrazione del messaggio
        ReadStreamNumMsgID(reply,0,0,msg_id);
        ReadStreamMsgVal(reply,0,0,0,first_key);
        ReadStreamMsgVal(reply,0,0,1,clent_id);

        if (strcmp(first_key, "client_id") != 0){
            send_response_status(c2r, WRITE_STREAM,client_id, "BAD_REQUEST",msg_id, 0);
            freeReplyObject(reply);
            continue;
        }
        // recupero cronologia delle prenotazioni
        try
        {
            std::string history = get_booking_history(db,client_id);
            //Aggiungo la cronologia al flusso Redis
            RedisCommand(2r,"XADD %s * client_id %s history %s",WRITE_STREAM, clien_id, history.c_str());
        }
        catch(const std::exception &e)
        {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
        }
        
        freeReplyObject(reply);
    }
    db.finish();
    return 0;
}
