#include "main.h"  // Include il file di intestazione che contiene le definizioni necessarie

int main() {
    redisContext *c2r;  // Puntatore al contesto Redis
    redisReply *reply;  // Puntatore alla risposta Redis

    PGresult *query_res;  // Risultato della query PostgreSQL

    std::string query;  // Stringa per la query SQL

    // Array di caratteri per vari dati
    char response[RESPONSE_LEN], msg_id[MESSAGE_ID_LEN], first_key[KEY_LEN], client_id[VALUE_LEN];

    // Connessione al database PostgreSQL
    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    // Connessione al server Redis
    c2r = redisConnect(REDIS_SERVER, REDIS_PORT);

    RichiestaPrenotazione* richiesta;  // Puntatore a un oggetto DeliveryPurchase

    while(1) {  // Ciclo infinito per elaborare i messaggi

        // Legge i messaggi dal gruppo Redis
        reply = RedisCommand(c2r, "XREADGROUP GROUP main amministrativo BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

        // Verifica la validità della risposta
        assertReply(c2r, reply);

        // Se non ci sono messaggi, continua il ciclo
        if (ReadNumStreams(reply) == 0) {
            continue;
        } 

        // Legge l'ID del messaggio
        ReadStreamNumMsgID(reply, 0, 0, msg_id);

        // Legge la chiave del primo campo del messaggio
        ReadStreamMsgVal(reply, 0, 0, 0, first_key);    // Indice del primo campo del messaggio = 0
        // Legge il valore del secondo campo del messaggio
        ReadStreamMsgVal(reply, 0, 0, 1, client_id);    // Indice del secondo campo del messaggio = 1

        // Verifica se la prima chiave è "client_id"
        if(strcmp(first_key, "client_id")){
            // Invia una risposta di richiesta non valida
            send_response_status(c2r, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        // Converte il messaggio in un oggetto DeliveryPurchase
        try{
            richiesta = RichiestaPrenotazione::from_stream(reply, 0, 0);
        }
        catch(std::invalid_argument exp){
            // Invia una risposta di richiesta non valida
            send_response_status(c2r, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        // Crea la query di inserimento
        query = //da capire come implementare la query
        
        // Esegue la query SQL
        query_res = db.RunQuery((char *) query.c_str(), false);

        // Verifica il risultato della query
        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            // Invia una risposta di errore del database
            send_response_status(c2r, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        // Invia una risposta di successo
        send_response_status(c2r, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, 0);
        
    }

    // Chiude la connessione al database
    db.finish();

    return 0;  // Termina il programma
}