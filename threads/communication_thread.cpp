//
// Created by pwiet on 16.05.2023.
//

#include "communication_thread.h"
#include "../main.h"
#include "../data/processData.h"
#include "../utils.h"
#include <vector>

void *communicationLoop(void *ptr){
    println("comm started")
    MPI_Status status;
    packet_t packet;
    while(true){
        MPI_Recv(&packet, 1, MPI_PACKET_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        processData.newLamportTime(packet.lamportTime);
        switch (status.MPI_TAG) {
            case REQ_AGRAFKA:
                println("Received REQ_AGRAFKA")
                sendPacket(status.MPI_SOURCE, ACK_AGRAFKA);
                break;
            case REQ_CELOWNIK:
                println("Received REQ_CELOWNIK")
                sendPacket(status.MPI_SOURCE, ACK_CELOWNIK);
                break;
            case ACK_AGRAFKA: {
                println("Received ACK_AGRAFKA from %d",status.MPI_SOURCE)
                queueAgrafkaType n{
                        status.MPI_SOURCE,packet.lamportTime,processData.hasCelownik
                };
                processData.addToVectorAgrafka(status.MPI_SOURCE, n);
                if (processData.checkVectorAgrafka()){
                    println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
                    processData.state = WAITING_CELOWNIK ;
                    condVarNotify();
                }
                break;
            }
            case ACK_CELOWNIK:
                println("Received ACK_CELOWNIK")
                break;
        }
    }
};