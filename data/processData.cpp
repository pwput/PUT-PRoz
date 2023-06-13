//
// Created by pwiet on 16.05.2023.
//

#include "processData.h"
#include "config.h"
#include "../main.h"
#include <bits/stdc++.h>
#include "../utils.h"

struct SenderClockRank {
    inline bool operator()(const queueItem &struct1, const queueItem &struct2) {
        if (struct1.senderClock == struct2.senderClock)
            return (struct1.senderRank < struct2.senderRank);
        else
            return (struct1.senderClock < struct2.senderClock);
    }
};


queueItem ProcessData::getNewQueueItem(
) {
    return {rank, lamportTime, hasCelownik, hasAgrafka};
}

struct AgrafkaSenderClockRank {
    inline bool operator()(const queueItem &struct1, const queueItem &struct2) {
        if (struct1.hasAgrafka && !struct2.hasAgrafka)
            return true;
        if (!struct1.hasAgrafka && struct2.hasAgrafka)
            return false;
        if (struct1.senderClock == struct2.senderClock)
            return (struct1.senderRank < struct2.senderRank);
        else
            return (struct1.senderClock < struct2.senderClock);
    }
};

void ProcessData::init(int rank, int size) {
    this->rank = rank;
    this->size = size;
    this->existingBronCount = 0;
    if (rank < GNOMY) {
        this->processType = GNOM;
        this->state = WAITING_AGRAFKA;
    } else {
        this->processType = SKRZAT;
        this->state = WAITING_BRON;
    }

    this->lamportTime = 0;

    this->hasCelownik = false;
    this->hasAgrafka = false;
}

void ProcessData::newLamportTime(int receivedLamportTime) {
    pthread_mutex_lock(&lamportMutex);
    if (receivedLamportTime > this->lamportTime)
        this->lamportTime = receivedLamportTime + 1;
    else
        this->lamportTime++;
    pthread_mutex_unlock(&lamportMutex);
}

char ProcessData::getProcessTypeLetter() {
    if (this->processType == SKRZAT)
        return 'S';
    else
        return 'G';
}


bool ProcessData::isSameProcessType(int senderRank) {
    if (rank < GNOMY)
        return (senderRank < GNOM);
    else return (senderRank >= GNOMY);
}

void ProcessData::addToVector(std::vector<queueItem> &vector, queueItem item) {
    bool added = false;
    for (int i = 0; i < vector.size(); ++i) {
        if (vector[i].senderRank == item.senderRank) {
            vector[i] = item;
            added = true;
        }
    }
    if (!added)
        vector.push_back(item);
}

bool ProcessData::checkVector(std::vector<queueItem> &vector, int limit, bool celownik) {
    if (vector.size() < size) return false;
    int myPlace = 0;
    if (celownik)
        std::sort(vector.begin(), vector.end(), AgrafkaSenderClockRank());
    else
        std::sort(vector.begin(), vector.end(), SenderClockRank());
    printVector(vector);
    for (int i = 0; i < vector.size(); ++i) {
        if (isSameProcessType(vector[i].senderRank))
        if (vector[i].senderRank == rank)
            myPlace = i;
    }
    printVector(vector);
    return (myPlace < limit -     this->existingBronCount);
}

void ProcessData::incLamportTime() {
    pthread_mutex_lock(&lamportMutex);
    this->lamportTime ++;
    pthread_mutex_unlock(&lamportMutex);
}
