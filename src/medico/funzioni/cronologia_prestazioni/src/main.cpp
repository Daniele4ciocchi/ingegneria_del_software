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
        redReply = RedisCommand(redConn, "XREADGROUP GROUP main paziente BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

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
        sprintf(query, "SELECT p.nome AS nome_paziente, p.cognome AS cognome_paziente, rp.specializzazione_nome AS specializzazione_visita, rp.giornoorariopren AS giorno_visita, rp.irich AS irich, pa.prestazioneavvenuta AS visita_avvenuta FROM prenotazioneaccettata pa, richiestaprenotazione rp, paziente paz, persona p WHERE pa.richiesta_id = rp.id AND rp.paziente_id = paz.id AND paz.cf = p.cf AND rp.medico_id = '%s';", medico_id);

        queryRes = db.RunQuery(query, true);
        
        if (PQresultStatus(queryRes) != PGRES_COMMAND_OK && PQresultStatus(queryRes) != PGRES_TUPLES_OK) {
            std::cout << "Errore query o paziente non trovato" << std::endl;
            send_response_status(redConn, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        std::list<RichiestaPrenotazione*> visite;

        for(int row = 0; row < PQntuples(queryRes); row++){
            RichiestaPrenotazione* visita;
            visita = new RichiestaPrenotazione(
                                PQgetvalue(queryRes, row, PQfnumber(queryRes, "nome_paziente")),
                                PQgetvalue(queryRes, row, PQfnumber(queryRes, "cognome_paziente")),
                                PQgetvalue(queryRes, row, PQfnumber(queryRes, "specializzazione_visita")),
                                PQgetvalue(queryRes, row, PQfnumber(queryRes, "giorno_visita")),
                                PQgetvalue(queryRes, row, PQfnumber(queryRes, "irich")),
                                PQgetvalue(queryRes, row, PQfnumber(queryRes, "visita_avvenuta"))); 
            visite.push_back(visita);
        }
        // nome_medico, cognome_medico, specializzazione, giornoorario, prestazioneavvenuta 
        send_response_status(redConn, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, PQntuples(queryRes));
        
        for(int row = 0; row < visite.size(); row++){

            RichiestaPrenotazione *v = visite.front();

            visite.pop_front();

            redReply = RedisCommand(redConn, "XADD %s * row %d nome %s cognome %s specializzazione %s giorno %s prestazioneavvenuta %s", 
                                 WRITE_STREAM, row,  v->paziente_id, v->medico_id, v->amministrativo_id, v->specializzazione_nome, v->giornoorariopren);
            assertReplyType(redConn, redReply, REDIS_REPLY_STRING);
            freeReplyObject(redReply);
            
        }
 
    }

    db.finish();

    return 0;
}
