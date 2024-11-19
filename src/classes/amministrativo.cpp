#include "amministrativo.h"

Amministrativo::Amministrativo(char* nome_amministrativo, char* email_amministrativo){

    nome = (char*) malloc(sizeof(char) * PRMTRSIZE);
    mail = (char*) malloc(sizeof(char) * PRMTRSIZE);

    strcpy(nome, nome_amministrativo);
    strcpy(mail, mail_amministrativo);
}

Amministrativo::~Amministrativo(){
    free(nome);
    free(mail);
}

Amministrativo* Amministrativo::from_stream(redisReply* reply, int stream_num, int msg_num){
    
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

    return new Amministrativo(nome, mail);
}