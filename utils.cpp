//
// Created by pwiet on 16.05.2023.
//

#include <iostream>
#include "utils.h"
#include "data/config.h"
#include "main.h"

using namespace std;

void printVector(std::vector<queueItem> vec) {
    for (int i = 0; i < vec.size(); ++i) {
  //      println("r:%d c:%d ha:%d hc:%d", vec[i].senderRank, vec[i].senderClock, vec[i].hasAgrafka, vec[i].hasCelownik);
    }
}

void sendPacket(int destination, int tag) {
    packet_t response;
    //actualization zegara i wypełnienie pól pakietu
    pthread_mutex_lock(&lamportMutex);

    response.lamportTime = processData.lamportTime;
    response.hasAgrafka = processData.hasAgrafka;
    response.hasCelownik = processData.hasCelownik;

    pthread_mutex_unlock(&lamportMutex);

    // wysłanie ACK
  //  debugln("send %s(t:%d, ha:%d, hc:%d) to rank = %d", MessageText[tag].c_str(), response.lamportTime, response.hasAgrafka,response.hasCelownik, destination);
    MPI_Send(&response, 1, MPI_PACKET_T, destination, tag, MPI_COMM_WORLD);
}

void sendPacketToAll(int tag) {
    for (int i = 0; i < processData.size; ++i)
        sendPacket(i, tag);
}
