#ifndef MAIN_H
#define MAIN_H

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
#include "../../../../classes/src/richiesta_prenotazione.h"
#include "../../../../utils/src/utils.h"
#include "../../../../utils/src/const.h"

/* Constants */
#define READ_STREAM "add-address-in"
#define WRITE_STREAM "add-address-out"

#define REDIS_SERVER "localhost"
#define REDIS_PORT 6379

#define POSTGRESQL_SERVER "localhost"
#define POSTGRESQL_PORT "5432"
#define POSTGRESQL_USER "customer"
#define POSTGRESQL_PSW "customer"
#define POSTGRESQL_DBNAME "ecommerce"

using namespace std;

/* Function declarations */
void assertReply(redisContext *c, redisReply *reply);
void send_response_status(redisContext *c, const char *stream, const char *client_id, const char *status, const char *msg_id, int ack);

#endif