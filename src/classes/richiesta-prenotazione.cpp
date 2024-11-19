#include "richiesta-prenotazione.h"

RichiestaPrenotazione::RichiestaPrenotazione(
                                char* nome_paziente, 
                                char* mail_paziente, 
                                char* tipo_visita,
                                char* data_visita,
                                char* data_prenotazione,
                                char* nome_medico
                                char* mail_medico
                                ){

    nome_p      = (char*) malloc(sizeof(char) * PRMTRSIZE); //nome paziente
    mail_p      = (char*) malloc(sizeof(char) * PRMTRSIZE); //mail paziente
    tipo_v      = (char*) malloc(sizeof(char) * PRMTRSIZE); //tipo visita
    data_v      = (char*) malloc(sizeof(char) * PRMTRSIZE); //data visita
    data_pr     = (char*) malloc(sizeof(char) * PRMTRSIZE); //data prenotazione
    nome_m      = (char*) malloc(sizeof(char) * PRMTRSIZE); //nome medico
    mail_m      = (char*) malloc(sizeof(char) * PRMTRSIZE); //mail medico


    strcpy(nome_p, nome_paziente);
    strcpy(mail_p, mail_paziente);
    strcpy(tipo_v, tipo_visita);
    strcpy(data_v, data_visita);
    strcpy(data_pr, data_prenotazione);
    strcpy(nome_m, nome_medico);
    strcpy(email_m, email_medico);


}

RichiestaPrenotazione::~RichiestaPrenotazione(){
    free(nome_p);
    free(mail_p);
    free(tipo_v);
    free(data_v);
    free(data_pr);
    free(nome_m);
    free(email_m);
}

// nella funzione di sotto bisogna defnire bene i campiin quanto non sono sicuro 
// che rispecchino quelli del database redis 

RichiestaPrenotazione* RichiestaPrenotazione::from_stream(redisReply* reply, int stream_num, int msg_num){
    
    char key[KEYSIZE];
    char value[PRMTRSIZE];

    char nome_p[PRMTRSIZE];
    char mail_p[PRMTRSIZE];
    char tipo_v[PRMTRSIZE];
    char data_v[PRMTRSIZE];
    char nome_m[PRMTRSIZE];
    char mail_m[PRMTRSIZE];

    char* data_pr;
    auto current_timestamp = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(current_timestamp);
    date = std::ctime(&current_time);

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num +=  2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);
        
        if (!strcmp(key, "nome_paziente")) {
            snprintf(nome_p, PRMTRSIZE, "%s", value);

        } else if (!strcmp(key, "mail_paziente")) {
            snprintf(mail_p, PRMTRSIZE, "%s", value);

        } else if (!strcmp(key, "tipo_visita")){
            snprintf(tipo_v, PRMTRSIZE, "%s", value);

        } else if (!strcmp(key, "data_visita")){
            snprintf(data_v, PRMTRSIZE, "%s", value);
            
        } else if (!strcmp(key, "nome_medico")){
            snprintf(nome_m, PRMTRSIZE, "%s", value);
            
        } else if (!strcmp(key, "mail_medico")){
            snprintf(mail_m, PRMTRSIZE, "%s", value);
            
        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    return new RichiestaPrenotazione(name_p, mail_p, tipo_v, data_v, datapr, nome_m, mail_m);
}