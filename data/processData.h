//
// Created by pwiet on 16.05.2023.
//

#ifndef UNTITLED1_DATA_H
#define UNTITLED1_DATA_H

#include "../common.h"
#include <vector>


struct queueType{
    int senderRank;
    int senderClock;
};

struct queueAgrafkaType : queueType{
    bool hasCelownik;
};

struct queueCelownikType : queueType{
    bool hasAgrafka;
};

struct queueBronType: queueType{};

class ProcessData {
public:
    ProcessType processType;
    State state;

    int rank;
    int size;
    int lamportTime;

    bool hasAgrafka;
    bool hasCelownik;

    std::vector<queueAgrafkaType> agrafkaQueue;
    std::vector<queueCelownikType> celownikQueue;
    std::vector<queueBronType> bronQueue;

    void init(int rank, int size);
    char getProcessTypeLetter();
    void newLamportTime(int receivedLamportTime);

    void addToVectorAgrafka(int senderRank, queueAgrafkaType item);
    void addToVectorCelownik(int senderRank, queueCelownikType item);
    void addToVectorBron(int senderRank, queueBronType item);

    bool checkVectorAgrafka();
    bool checkVectorCelownik();
    bool checkVectorBron();
};


#endif //UNTITLED1_DATA_H
