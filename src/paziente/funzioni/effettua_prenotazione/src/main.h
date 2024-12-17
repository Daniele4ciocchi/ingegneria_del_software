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

/* Local libraries */

#include "../../../../service/database/dbConnection.h"
#include "../../../../service/redis/redisConnection.h"

#include "../../../../classes/src/richiesta_prenotazione.h"
#include "../../../../utils/src/utils.h"
#include "../../../../shared/standard.h"

/* Constants */

#define READ_STREAM "effettua_prenotazione-in"
#define WRITE_STREAM "effettua_prenotazione-out"

#define REDIS_SERVER "localhost"
#define REDIS_PORT 6379

#define POSTGRESQL_SERVER "localhost"
#define POSTGRESQL_PORT "5432"
#define POSTGRESQL_USER "paziente"
#define POSTGRESQL_PSW "paziente"
#define POSTGRESQL_DBNAME "prenotazionimediche"

using namespace std;

#endif