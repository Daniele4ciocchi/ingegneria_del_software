#include "main.h"
bool handle_redis_error(redisContext* c2r,redisReply* reply, const char*client_id, const char* msg_id, const char* status) {
    if (!reply || reply->type == REDIS_REPLY_ERROR){
        send_response_status(c2r,WRITE_STREAM, client_id, status, msg_id, 0);
        if (reply) freeReplyObject(reply);
        return false;
    }
    return true;
}
int main() {
    redisContext *c2r;
    redisReply *reply;
    PGresult *query_res;

    char msg_id[MESSAGE_ID_LEN],first_key[KEY_LEN], client_id[VALUE_LEN];
    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    c2r = reisConnect(REDIS_SERVER, REDIS_PORT);
    if (c2r == nullptr || c2r->err){
        std::cerr << "Failed to connect to Redis server: " <<2r->errstr << std::endl;
        return -1;
    }
    while(1){
        reply = RedisCommand(c2r, "XREADGROUP GROUP main costumer BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);
        if (!handle_redis_error(c2r,reply,client_id,msg_id,"REDIS_ERROR")){
            continue;
        }
        if (ReadNumStreams(reply) == 0) {
            continue;
        }
        ReadStreamNumMsgID(reply, 0,0,msg_id);
        ReadStreamMsgVal(reply 0,0,0, first_key);
        ReadStreamMsgVal(reply, 0,0,1, client_id);

        if (strcmp(first_key,"cient_id")!= 0) {
            send_response_status(c2r,WRITE_STREAM,client_id,"BAD_REQUEST", msg_id, 0);
        }
    }
}