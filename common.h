//
// Created by pwiet on 16.05.2023.
//

#ifndef OMMON_H
#define OMMON_H

#include <string>

typedef struct {
    int lamportTime;
    bool hasAgrafka;
    bool hasCelownik;
} packet_t;

enum ProcessType{
    GNOM,
    SKRZAT,
    ALL
};

enum Types{
    Agrafka,

};

enum State{
    KILLING, //skrzat
    WAITING_BRON, //skrzat
    MAKING_BRON, //gnom
    WAITING_AGRAFKA, //gnom
    WAITING_CELOWNIK //gnom
};

enum Message{
    REQ_AGRAFKA=0,
    ACK_AGRAFKA,
    REQ_CELOWNIK,
    ACK_CELOWNIK,
    ACK_BRON,
    REQ_BRON,
    RELEASE_AGRAFKA,
    RELEASE_CELOWNIK
    
};

std::string MessageText[8] = {
"REQ_AGRAFKA",
"ACK_AGRAFKA",
"REQ_CELOWNIK",
"ACK_CELOWNIK",
"ACK_BRON",
"REQ_BRON",
"RELEASE_AGRAFKA",
"RELEASE_CELOWNIK"
};


#endif //COMMON_H
