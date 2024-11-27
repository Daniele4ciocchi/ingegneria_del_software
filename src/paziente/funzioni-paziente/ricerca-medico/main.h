#ifndef MAIN_H
#define MAIN_H

#include <hiredis/hiredis.h>
#include <libpq-fe.h>
#include "../db/DbConnection.h"
#include "../redis/RedisCommand.h"
#include "../redis/RedisUtils.h"
#include "../shared/standard.h"
#include "../classes/medico.h"

void send_response_status(redisContext *redConn, const char *stream, const char *client_id, const char *status, const char *msg_id, int num_results);

#endif // MAIN_H