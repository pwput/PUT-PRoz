//
// Created by pwiet on 16.05.2023.
//

#include <iostream>
#include "utils.h"
#include "data/config.h"
#include "main.h"

using namespace std;

void printVector(std::vector<queueCelownikType> vec){
    cout<<"vectror c"<<endl;
    for (int i = 0; i < vec.size(); ++i) {
        cout<<vec[i].senderRank<< " "<<vec[i].senderClock<< " "<<vec[i].hasAgrafka<<endl;
    }
}
void printVector(std::vector<queueAgrafkaType> vec){
    println("vector A");
    for (int i = 0; i < vec.size(); ++i) {
        cout<<vec[i].senderRank<< " "<<vec[i].senderClock<< " "<<vec[i].hasCelownik<<endl;
    }
}
void printVector(std::vector<queueBronType> vec){
    cout<<"vectror B"<<endl;

    for (int i = 0; i < vec.size(); ++i) {
        cout<<vec[i].senderRank<< " "<<vec[i].senderClock<<endl;
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
    processData.lamportTime++;

    response.lamportTime = processData.lamportTime;
    response.hasAgrafka = processData.hasAgrafka;
    response.hasCelownik = processData.hasCelownik;

    pthread_mutex_unlock(&lamportMutex);

    // wysłanie ACK
    if (DEBUG) println("send ACK_%d(time = %d) to rank = %d", tag,response.lamportTime, destination);
    MPI_Send( &response, 1, MPI_PACKET_T, destination, tag, MPI_COMM_WORLD);
}

void sendPacketToAll(int tag){
    for (int i = 0; i < processData.size; ++i) {
        if (i!=processData.rank)
            sendPacket(i, tag);
    }

}