#ifndef utils_h
#define utils_h

/* System libraries */

#include <string>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <sstream>
#include "../../lib/postgresql/libpq-fe.h"

/* Local libraries */

#include "../../lib/con2redis/src/con2redis.h"
#include "../../lib/con2db/pgsql.h"


/* Procedures */

void send_response_status(redisContext* c2r, const char *stream, const char *client_id, const char *resp_status, const char *msg_id, const int nrows);

#endif 
