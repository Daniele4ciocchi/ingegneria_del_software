#ifndef PRENOTAZIONE_ACCETTATA_H
#define PRENOTAZIONE_ACCETTATA_H

#include <ctime>
#include <stdexcept>
#include <string.h>

class PrenotazioneAccettata {
public:
    int richiesta_id;
    std::tm iaccet;
    bool prestazioneavvenuta;

    PrenotazioneAccettata(int richiesta_id, const std::tm& iaccet, bool prestazioneavvenuta);
    ~PrenotazioneAccettata();

    static PrenotazioneAccettata* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif // PRENOTAZIONE_ACCETTATA_H