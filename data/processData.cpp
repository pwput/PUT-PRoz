//
// Created by pwiet on 16.05.2023.
//

#include "processData.h"
#include "config.h"
#include "../main.h"
#include <bits/stdc++.h>
#include "../utils.h"

struct senderClockDesc
{
    inline bool operator() (const queueType& struct1, const queueType& struct2)
    {
        if (struct1.senderClock == struct2.senderClock)
            return (struct1.senderRank < struct2.senderRank);
        else
            return (struct1.senderClock < struct2.senderClock);
    }
};

void ProcessData::init(int rank, int size){
    this->rank = rank;
    this->size = size;
    this->existingBronCount = 0;
    if (rank< GNOMY){
        this->processType = GNOM;
        this->state = WAITING_AGRAFKA;
    }
    else{
        this ->processType = SKRZAT;
        this->state = WAITING_BRON;
    }

    this->lamportTime =0;

    this->hasCelownik = false;
    this->hasAgrafka = false;
}

void ProcessData::newLamportTime(int receivedLamportTime){
    pthread_mutex_lock(&lamportMutex);
    if (receivedLamportTime > this->lamportTime)
        this->lamportTime = receivedLamportTime + 1;
    else
        this->lamportTime ++;
    pthread_mutex_unlock(&lamportMutex);
}

char ProcessData::getProcessTypeLetter(){
    if(this->processType == SKRZAT)
        return 'S';
    else
        return 'G';
}


void ProcessData::addToVectorAgrafka(int senderRank, queueAgrafkaType item) {
    lockStateMutex();
    bool added = false;
    for (int i = 0; i < agrafkaQueue.size(); ++i) {
        if (agrafkaQueue[i].senderRank == senderRank){
            agrafkaQueue[i] = item;
            added = true;
        }
    }
    if (!added)
        agrafkaQueue.push_back(item);
    unlockStateMutex();
}
void ProcessData::addToVectorCelownik(int senderRank, queueCelownikType item) {
    lockStateMutex();
    bool added = false;
    for (int i = 0; i < celownikQueue.size(); ++i) {
        if (celownikQueue[i].senderRank == senderRank){
            celownikQueue[i] = item;
            added = true;
        }
    }
    if (!added)
        celownikQueue.push_back(item);
    unlockStateMutex();
}
void ProcessData::addToVectorBron(int senderRank, queueBronType item) {
    lockStateMutex();
    bool added = false;
    for (int i = 0; i < bronQueue.size(); ++i) {
        if (bronQueue[i].senderRank == senderRank){
            bronQueue[i] = item;
            added = true;
        }
    }
    if (!added)
        bronQueue.push_back(item);
    unlockStateMutex();
}

bool ProcessData::checkVectorAgrafka(){
    lockStateMutex();
    int myplace  = 0;
    std::sort(agrafkaQueue.begin(), agrafkaQueue.end(), senderClockDesc());

    for (int i = 0; i < agrafkaQueue.size(); ++i) {
        if(agrafkaQueue[i].senderClock < lamportTime)
            myplace ++;
    }
    unlockStateMutex();
    return (myplace < AGRAFKI - 1);
}
bool ProcessData::checkVectorCelownik(){
    lockStateMutex();
    int myplace  = 0;
    printVector(celownikQueue);
    std::sort(celownikQueue.begin(), celownikQueue.end(), senderClockDesc());
    for (int i = 0; i < celownikQueue.size(); ++i) {
        if(celownikQueue[i].senderClock < lamportTime)
            myplace ++;
    }
    unlockStateMutex();
    return (myplace < CELOWNIKI - 1);
}
bool ProcessData::checkVectorBron(){
    lockStateMutex();

    unlockStateMutex();
}

