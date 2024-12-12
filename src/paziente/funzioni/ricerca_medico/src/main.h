#ifndef main_h
#define main_h

/* System libraries */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/times.h>
#include <cassert>
#include <cerrno>
#include <iostream>

/* Local libraries */

#include "../../../../lib/con2db/pgsql.h"
#include "../../../../lib/con2redis/src/con2redis.h"

#include "../../../../classes/src/medico.h"
#include "../../../../utils/src/utils.h"
#include "../../../../utils/src/const.h"

/* Constants */

// Stream per la comunicazione Redis
#define READ_STREAM "medico-search-in"
#define WRITE_STREAM "medico-search-out"

// Configurazione Redis
#define REDIS_SERVER "localhost"
#define REDIS_PORT 6379

// Configurazione PostgreSQL
#define POSTGRESQL_SERVER "localhost"
#define POSTGRESQL_PORT "5432"
#define POSTGRESQL_USER "paziente_user"
#define POSTGRESQL_PSW "paziente_password"
#define POSTGRESQL_DBNAME "prenotazionimediche"

// Dimensioni massime per chiavi e valori
#define QUERYSIZE 512
#define MSGIDSIZE 64
#define KEYSIZE 128
#define VALUESIZE 256
#define PRMTRSIZE 64

/* Function declarations */

// Funzione per inviare lo stato della risposta al cliente
void send_response_status(redisContext *redConn, const char *stream, const char *client_id, const char *status, const char *msg_id, int num_results);

#endif
