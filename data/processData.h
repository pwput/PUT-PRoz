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
    ProcessType processType;
    State state;

    int existingBronCount;
    int rank;
    int size;
    int lamportTime;

    bool hasAgrafka;
    bool hasCelownik;

    std::vector<queueItem> agrafkaReqQueue;
    std::vector<queueItem> celownikReqQueue;
    std::vector<queueItem> bronReqQueue;

    std::vector<queueItem> agrafkaAck;
    std::vector<queueItem> celownikAck;
    std::vector<queueItem> bronAck;

    queueItem getNewQueueItem();

    void init(int rank, int size);

    char getProcessTypeLetter();

    int countOfProceses(ProcessType type){
        switch (type) {
            case ProcessType::GNOM: return GNOMY;
            case ProcessType::SKRZAT: return size - GNOM;
        }
    };
    void newLamportTime(int receivedLamportTime);
    void incLamportTime();
    void addToVector(std::vector<queueItem> &vector, queueItem item);
    bool canIHaveAgrafka();
    bool canIHaveCelownik();
    bool canIHaveBron();
private:
    bool canIHave(std::vector<queueItem> &reqVector, std::vector<queueItem> &ackVector,int limit, int neededAck, bool celownik = false);
    bool isSameProcessType(int senderRank);
};


#endif //UNTITLED1_DATA_H
