#include "utils.h"

void send_response_status(redisContext* c2r, const char *stream, const char *client_id, const char *resp_status, const char *msg_id, const int nrows){
    redisReply *reply;

    // LOG
    printf("Response: %s %s %s %s %d\n", stream, client_id, resp_status, msg_id, nrows);

    reply = RedisCommand(c2r, "XADD %s * client_id %s resp_status %s num_rows %d", stream, client_id, resp_status, nrows);
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);
    freeReplyObject(reply);
}

void print_queryResult(PGresult *queryRes) {
    if (!queryRes) {
        fprintf(stderr, "Risultato della query è NULL\n");
        return;
    }

    if (PQresultStatus(queryRes) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Query fallita: %s\n", PQresultErrorMessage(queryRes));
        PQclear(queryRes);
        return;
    }

    int nFields = PQnfields(queryRes);
    int nTuples = PQntuples(queryRes);

    printf("Numero di colonne: %d\n", nFields);
    printf("Numero di righe: %d\n", nTuples);

    for (int i = 0; i < nTuples; i++) {
        for (int j = 0; j < nFields; j++) {
            printf("%s\t", PQgetvalue(queryRes, i, j));
        }
        printf("\n");
    }

    PQclear(queryRes);
}

std::string replace_substring(std::string input, const std::string& target, const std::string& replacement) {
    size_t pos = input.find(target);

    while (pos != std::string::npos) {
        input.replace(pos, target.length(), replacement);
        pos = input.find(target, pos + replacement.length());
    }

    return input;
}

std::string get_current_timestamp_as_string() {
    // Get current time point
    auto now = std::chrono::system_clock::now();

    // Convert to time_t (seconds since epoch)
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    // Convert time_t to tm structure
    std::tm *localTime = std::localtime(&currentTime);

    // Format tm structure into a string
    std::ostringstream oss;
    oss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");

    // Get the resulting string
    return oss.str();
}
