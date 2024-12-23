#ifndef handler_h
#define handler_h

/* System libraries */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/times.h>
#include <cassert>
#include <cerrno>
#include <iostream>

/* Local libraries */

#include "../../lib/con2redis/src/con2redis.h"
#include "../../lib/con2db/pgsql.h"
#include "../../utils/src/const.h"

/* Classes */
class Handler{
    public:
        Handler(const char* redis_ip, int redis_port, std::string req_types[], int num_req_types);

        bool send_to_funzioni(int client_id, std::string cmd);
        bool read_from_funzioni(std::string* out_str_ptr, int* client_id_ptr);

    private:
        redisContext *c2r;
        std::string* types;
        int num_types;
        
        void init_streams();
};

#endif