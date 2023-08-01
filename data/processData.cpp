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
    if (rank < SKRZATY) {
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

bool ProcessData::canIHaveAgrafka(){
    return canIHave(this->agrafkaReqQueue, this->agrafkaAck, AGRAFKI, SKRZATY);
};
bool ProcessData::canIHaveCelownik(){
    return canIHave(this->celownikReqQueue, this->celownikAck, CELOWNIKI, SKRZATY, true);
};

bool ProcessData::canIHave(std::vector<queueItem> &reqVector, std::vector<queueItem> &ackVector,int limit, int neededAck, bool celownik){
    if (ackVector.size() < neededAck) return false;
    debugln("I have all answers");

    if (celownik)
        std::sort(reqVector.begin(), reqVector.end(), AgrafkaSenderClockRank());
    else
        std::sort(reqVector.begin(), reqVector.end(), SenderClockRank());

    int myPlaceInReqVector = 0;
    int myTimeOfRequest = 0;
    for (int i = 0; i < reqVector.size(); ++i) {
        if (reqVector[i].senderRank == this->rank){
            myPlaceInReqVector = i;
            myTimeOfRequest = reqVector[i].senderClock;
        }
    }

    printVector(reqVector);
    debugln("My place in reqVector: %d, of %zu", myPlaceInReqVector, reqVector.size());

    bool areAllAckTimesWorst = true;
    for (auto ack : ackVector){
        if (ack.senderClock < myTimeOfRequest)
            areAllAckTimesWorst = false;
    }
    if (!areAllAckTimesWorst) return false;

    debugln("All Revived Ack Times are worst")

    return (myPlaceInReqVector < limit );
}

void ProcessData::incLamportTime() {
    pthread_mutex_lock(&lamportMutex);
    this->lamportTime ++;
    pthread_mutex_unlock(&lamportMutex);
}
