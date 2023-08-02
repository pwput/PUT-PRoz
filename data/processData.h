//
// Created by pwiet on 16.05.2023.
//

#ifndef UNTITLED1_DATA_H
#define UNTITLED1_DATA_H

#include "../common.h"
#include "config.h"
#include <vector>


struct queueItem {
    int senderRank;
    int senderClock;
    bool hasCelownik;
    bool hasAgrafka;

    queueItem(int senderRank, int senderClock, bool hasCelownik, bool hasAgrafka) {
        this->senderRank = senderRank;
        this->senderClock = senderClock;
        this->hasCelownik = hasCelownik;
        this->hasAgrafka = hasAgrafka;
    }
};


class ProcessData {
public:
    State state;

    int rank;
    int size;
    int lamportTime;

    bool hasAgrafka;
    bool hasCelownik;

    std::vector<queueItem> agrafkaReqQueue;
    std::vector<queueItem> celownikReqQueue;

    std::vector<queueItem> agrafkaAck;
    std::vector<queueItem> celownikAck;

    queueItem getNewQueueItem();

    void init(int rank, int size);

    char getProcessTypeLetter();


    void newLamportTime(int receivedLamportTime);
    void incLamportTime();
    void addToVector(std::vector<queueItem> &vector, queueItem item);
    void removeFromVector(std::vector<queueItem> &vector, int rank);
    bool canIHaveAgrafka();
    bool canIHaveCelownik();
private:
    bool canIHave(std::vector<queueItem> &reqVector, std::vector<queueItem> &ackVector,int limit, int neededAck, bool celownik = false);
};


#endif //UNTITLED1_DATA_H
