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

#include "../../../../lib/con2db/pgsql.h"
#include "../../../../lib/con2redis/src/con2redis.h"

#include "../../../../classes/src/prenotazione_rifiutata.h"
#include "../../../../utils/src/utils.h"
#include "../../../../utils/src/const.h"

/* Constants */

#define READ_STREAM "rifiuta_richpren-in"
#define WRITE_STREAM "rifiuta_richpren-out"

#define REDIS_SERVER "localhost"
#define REDIS_PORT 6379

#define POSTGRESQL_SERVER "localhost"
#define POSTGRESQL_PORT "5432"
#define POSTGRESQL_USER "amministrativo"
#define POSTGRESQL_PSW "amministrativo"
#define POSTGRESQL_DBNAME "prenotazionimediche"

using namespace std;

#endif