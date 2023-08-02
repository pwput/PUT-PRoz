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

enum State{
    MAKING_BRON, //gnom
    WAITING_AGRAFKA, //gnom
    WAITING_CELOWNIK //gnom
};

enum Message{
    REQ_AGRAFKA=0,
    ACK_AGRAFKA,
    REQ_CELOWNIK,
    ACK_CELOWNIK,
    RELEASE
};

std::string MessageText[8] = {
"REQ_AGRAFKA",
"ACK_AGRAFKA",
"REQ_CELOWNIK",
"ACK_CELOWNIK",
"RELEASE"
};


#endif //COMMON_H
