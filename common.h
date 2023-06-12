//
// Created by pwiet on 16.05.2023.
//

#ifndef OMMON_H
#define OMMON_H

typedef struct {
    int lamportTime;
    bool hasAgrafka;
    bool hasCelownik;
} packet_t;

enum ProcessType{
    GNOM,
    SKRZAT
};

enum State{
    KILLING, //skrzat
    WAITING_BRON, //skrzat
    MAKING_BRON, //gnom
    WAITING_AGRAFKA, //gnom
    WAITING_CELOWNIK //gnom
};

enum GnomMessage{
    REQ_AGRAFKA,
    ACK_AGRAFKA,
    REQ_CELOWNIK,
    ACK_CELOWNIK,
};

enum SkrzatMessage{
    REQ_BRON,
    RELEASE_AGRAFKA,
    RELEASE_CELOWNIK,
};



#endif //COMMON_H
