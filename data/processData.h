//
// Created by pwiet on 16.05.2023.
//

#ifndef UNTITLED1_DATA_H
#define UNTITLED1_DATA_H

#include "../common.h"
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
    ProcessType processType;
    State state;

    int existingBronCount;
    int rank;
    int size;
    int lamportTime;

    bool hasAgrafka;
    bool hasCelownik;

    std::vector<queueItem> agrafkaQueue;
    std::vector<queueItem> celownikQueue;
    std::vector<queueItem> bronQueue;

    queueItem getNewQueueItem();

    void init(int rank, int size);

    char getProcessTypeLetter();

    void newLamportTime(int receivedLamportTime);
    void incLamportTime();

    void addToVector(std::vector<queueItem> &vector, queueItem item);

    bool checkVector(std::vector<queueItem> &vector, int limit, bool celownik = false);

    bool isSameProcessType(int senderRank);
};


#endif //UNTITLED1_DATA_H
