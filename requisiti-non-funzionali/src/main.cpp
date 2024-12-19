#include "main.h"
using namespace std;

int micro_sleep(long usec)
{
    struct timespec ts;
    int res;

    if (usec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = usec / 1000000;
    ts.tv_nsec = (usec % 1000000) * 1000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;  
}

int main() {
    Con2DB logdb = Con2DB(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    PGresult *queryRes;

    char query[QUERY_LEN];

    while(1) {
        sprintf(query, "SELECT EXTRACT(EPOCH FROM AVG(idisconnection - iconnection)) * 1000 as avg FROM client WHERE idisconnection IS NOT NULL");

        queryRes = logdb.RunQuery(query, true);

        if ((PQresultStatus(queryRes) != PGRES_COMMAND_OK && PQresultStatus(queryRes) != PGRES_TUPLES_OK) || PQntuples(queryRes) <= 0) {
            printf("DB_ERROR\n");
            continue;
        }

        char* average = PQgetvalue(queryRes, 0, PQfnumber(queryRes, "avg"));

        if(strlen(average)==0){
            sprintf(average, "0");
        }

        char response_status[8];

        if (atof(average) <= MAX_CONNECTION_TIME_AVG) {
            sprintf(response_status, "SUCCESS");
        } else {
            sprintf(response_status, "ERROR");
        }

        sprintf(query, "INSERT INTO sessionstatistic(type, iend, value, response_status) VALUES ('Session', CURRENT_TIMESTAMP, %s, \'%s\')", average, response_status);

        queryRes = logdb.RunQuery(query, false);

        if (PQresultStatus(queryRes) != PGRES_COMMAND_OK && PQresultStatus(queryRes) != PGRES_TUPLES_OK) {
            printf("DB_ERROR\n");
            continue;
        }
        cout << "dati connessione inseriti" << endl;

        sprintf(query, "SELECT EXTRACT(EPOCH FROM AVG(iresponse - irequest)) * 1000 as avg FROM communication WHERE iresponse IS NOT NULL");

        queryRes = logdb.RunQuery(query, true);

        if ((PQresultStatus(queryRes) != PGRES_COMMAND_OK && PQresultStatus(queryRes) != PGRES_TUPLES_OK) || PQntuples(queryRes) <= 0) {
            printf("DB_ERROR\n");
            continue;
        }
        
        average = PQgetvalue(queryRes, 0, PQfnumber(queryRes, "avg"));

        if(strlen(average) == 0){
            sprintf(average, "0");
        }

        if (atof(average) <= MAX_RESPONSE_TIME_AVG) {
            sprintf(response_status, "SUCCESS");
        } else {
            sprintf(response_status, "ERROR");
        }

        sprintf(query, "INSERT INTO sessionstatistic(type, iend, value, response_status) VALUES ('Response', CURRENT_TIMESTAMP, %s, \'%s\')", average, response_status);

        queryRes = logdb.RunQuery(query, false);

        if (PQresultStatus(queryRes) != PGRES_COMMAND_OK && PQresultStatus(queryRes) != PGRES_TUPLES_OK) {
            printf("DB_ERROR\n");
            continue;
        }

        printf("statistiche ndi connessione - media: %s ms, tipo: SESSIONE, stato: %s\n", average, response_status);

        micro_sleep(60000000);
    }

    logdb.finish();

    return 0;
}