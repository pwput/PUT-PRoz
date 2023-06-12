//
// Created by pwiet on 16.05.2023.
//

#include "processData.h"
#include "config.h"
#include "../main.h"
#include "../utils.h"


void ProcessData::init(int rank, int size){
    this->rank = rank;
    this->size = size;
    if (rank< GNOMY){
        this->processType = GNOM;
        this->state = WAITING_AGRAFKA;
    }
    else{
        this ->processType = SKRZAT;
        this->state = WAITING_BRON;
    }
//    this->agrafkaQueue.resize(size);
//    this->celownikQueue.resize(size);
//    this->bronQueue.resize(size);
    
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
    if (agrafkaQueue.)
    this->agrafkaQueue[senderRank] = item;
    unlockStateMutex();
}
void ProcessData::addToVectorCelownik(int senderRank, queueCelownikType item) {
    lockStateMutex();
    this->celownikQueue[senderRank] = item;
    unlockStateMutex();
}
void ProcessData::addToVectorBron(int senderRank, queueBronType item) {
    lockStateMutex();
    this->bronQueue[senderRank] = item;
    unlockStateMutex();
}


bool ProcessData::checkVectorAgrafka(){
    lockStateMutex();
    bool ret  = true;
    printVector(agrafkaQueue);

    for (int i = 0; i < agrafkaQueue.size(); ++i) {
        if (i != rank)
        if(agrafkaQueue[i].senderClock < 1)
            ret = false;
    }
    this->agrafkaQueue;
    unlockStateMutex();

    return ret;
}
bool ProcessData::checkVectorCelownik(){
    lockStateMutex();

    unlockStateMutex();
}
bool ProcessData::checkVectorBron(){
    lockStateMutex();

    unlockStateMutex();
}

