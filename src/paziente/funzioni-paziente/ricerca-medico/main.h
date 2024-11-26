#ifndef main_h
#define main_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/times.h>
#include <string>
#include <cassert>
#include <cerrno>

// librerie locali

#include "../../../service/database/dbConnection.h"
#include "../../../service/redis/redisConnection.h"
#include "../../../classes/ordered-product.h"
#include "../../../shared/standard.h"

// costanti

#define READ_STREAM "make-order-in" // da modificare
#define WRITE_STREAM "make-order-out"// da modificare

#define REDIS_SERVER "localhost"
#define REDIS_PORT 6379

#define POSTGRESQL_SERVER "localhost"
#define POSTGRESQL_PORT "5432"
#define POSTGRESQL_USER "paziente"
#define POSTGRESQL_PSW "paziente"
#define POSTGRESQL_DBNAME "prenotazioni" // da modificare (nome database)

using namespace std;

#endif