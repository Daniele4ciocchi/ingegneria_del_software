#ifndef PERSONA_H
#define PERSONA_H

/* System libraries */
#include <string.h>
#include <stdexcept>

/* Local libraries */
#include "../../shared/standard.h"
#include "../../service/redis/redisConnection.h"

/* Classes */
class Persona {
public:
    char* cf = NULL;
    char* nome = NULL;
    char* cognome = NULL;
    char* nascita = NULL;


    Persona( char* cf, char* nome, char* cognome, char* nascita);
    ~Persona();

    static Persona* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif