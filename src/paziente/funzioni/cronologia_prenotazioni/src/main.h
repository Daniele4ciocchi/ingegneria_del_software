#ifndef MAIN_H
#define MAIN_H

/* System libraries */
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <cstring>
#include <cassert>

/* Local libraries */
#include "../../../../lib/con2db/pgsql.h"
#include "../../../../lib/con2redis/src/con2redis.h"
#include "../../../../utils/src/utils.h"
#include "../../../../utils/src/const.h"

/* Constants */
#define READ_STREAM "booking-history-in"
#define WRITE_STREAM "booking-history-out"

#define REDIS_SERVER "localhost"
#define REDIS_PORT 6379

#define POSTGRESQL_SERVER "localhost"
#define POSTGRESQL_PORT "5432"
#define POSTGRESQL_USER "cronologia_user"
#define POSTGRESQL_PSW "cronologia_password"
#define POSTGRESQL_DBNAME "prenotazionimediche"

#define MESSAGE_ID_LEN 128
#define KEY_LEN 64
#define VALUE_LEN 256

/* Function prototypes */
std::string get_booking_history(Con2DB &db, const std::string &client_id);
void send_response_status(redisContext *redConn, const char *stream, const char *client_id, const char *status, const char *msg_id, int num_results);

#endif // MAIN_H
