//
// Created by pwiet on 16.05.2023.
//

#include <iostream>
#include "utils.h"
#include "data/config.h"
#include "main.h"

using namespace std;

void printVector(std::vector<queueItem> vec){
    for (int i = 0; i < vec.size(); ++i) {
        println("%d %d %d %d",vec[i].senderRank,vec[i].senderClock,vec[i].hasCelownik,vec[i].hasAgrafka);
    }
}
void logDebug(string message) {
    if (DEBUG) cout << message << endl;
}

void log(string message){
    cout << message <<endl;
}

void sendPacket(int destination, int tag)
{
    packet_t response;
    //akualizacja zegara i wypełnienie pól pakietu
    pthread_mutex_lock(&lamportMutex);

    response.lamportTime = processData.lamportTime;
    response.hasAgrafka = processData.hasAgrafka;
    response.hasCelownik = processData.hasCelownik;

    pthread_mutex_unlock(&lamportMutex);

    // wysłanie ACK
    if (DEBUG) println("send %s(time = %d) to rank = %d", MessageText[tag].c_str(),response.lamportTime,destination);
    MPI_Send( &response, 1, MPI_PACKET_T, destination, tag, MPI_COMM_WORLD);
}

void sendPacketToAll(int tag){
    for (int i = 0; i < processData.size; ++i) {
        if (i!=processData.rank)
            sendPacket(i, tag);
    }

}