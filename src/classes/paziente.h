#ifndef PAZIENTE_H
#define PAZIENTE_H

/* System libraries */
#include <string.h>
#include <stdexcept>

/* Local libraries */
#include "../../utils/src/const.h"
#include "../../lib/con2redis/src/con2redis.h"

/* Classes */
class Paziente {
public:
    char* cf;
    char* nome;
    char* cognome;
    char* nascita;
    char* indirizzo;
    char* email;
    char* telefono;

    Paziente(char* cf_paziente, char* nome_paziente, char* cognome_paziente, char* nascita_paziente, char* indirizzo_paziente, char* email_paziente, char* telefono_paziente);
    ~Paziente();

    static Paziente* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif