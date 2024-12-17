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
#include "../../../../service/database/dbConnection.h"
#include "../../../../service/redis/redisConnection.h"

/* Constants */
#define READ_STREAM "prestazioni-history-in"
#define WRITE_STREAM "prestazioni-history-out"

#define REDIS_SERVER "localhost"
#define REDIS_PORT 6379

#define POSTGRESQL_SERVER "localhost"
#define POSTGRESQL_PORT "5432"
#define POSTGRESQL_USER "prestazioni_user"
#define POSTGRESQL_PSW "prestazioni_password"
#define POSTGRESQL_DBNAME "prenotazionimediche"

using namespace std;

#endif 
