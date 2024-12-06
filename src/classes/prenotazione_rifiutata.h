#ifndef PRENOTAZIONE_RIFIUTATA_H
#define PRENOTAZIONE_RIFIUTATA_H

#include <ctime>
#include <stdexcept>
#include <string.h>

class PrenotazioneRifiutata {
public:
    int richiesta_id;
    std::tm irif;
    int motivazione_id;

    PrenotazioneRifiutata(int richiesta_id, const std::tm& irif, int motivazione_id);
    ~PrenotazioneRifiutata();

    static PrenotazioneRifiutata* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif // PRENOTAZIONE_RIFIUTATA_H