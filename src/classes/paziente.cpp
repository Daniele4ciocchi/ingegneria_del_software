#include "paziente.h"

Paziente::Paziente(char* nome_paziente, char* email_paziente){

    nome = (char*) malloc(sizeof(char) * PRMTRSIZE);
    mail = (char*) malloc(sizeof(char) * PRMTRSIZE);

    strcpy(nome, nome_paziente);
    strcpy(mail, mail_paziente);
}

Paziente::~Paziente(){
    free(nome);
    free(mail);
}

Paziente* Paziente::from_stream(redisReply* reply, int stream_num, int msg_num){
    
    char key[KEYSIZE];
    char value[PRMTRSIZE];

    char nome[PRMTRSIZE];
    char mail[PRMTRSIZE];

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num +=  2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);
        
        if (!strcmp(key, "nome")) {
            snprintf(nome, PRMTRSIZE, "%s", value);

        } else if (!strcmp(key, "mail")) {
            snprintf(mail, PRMTRSIZE, "%s", value);

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    return new Customer(name, mail);
}