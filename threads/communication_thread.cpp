//
// Created by pwiet on 16.05.2023.
//

#include "communication_thread.h"
#include "../main.h"
#include "../data/processData.h"
#include "../utils.h"
#include <vector>
#include <sys/wait.h>
#include "../data/config.h"

void *communicationLoop(void *ptr) {
    println("comm started")
    MPI_Status status;
    packet_t packet;
    while (true) {
        MPI_Recv(&packet, 1, MPI_PACKET_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        processData.newLamportTime(packet.lamportTime);
        switch (status.MPI_TAG) {
            case REQ_AGRAFKA: {
                println("Received REQ_AGRAFKA from %d time %d", status.MPI_SOURCE,packet.lamportTime)
                lockStateMutex();
                queueItem newItem{status.MPI_SOURCE, packet.lamportTime, packet.hasCelownik, packet.hasAgrafka};
                processData.addToVector(processData.agrafkaReqQueue, newItem);
                sendPacket(status.MPI_SOURCE, ACK_AGRAFKA);
                unlockStateMutex();
                break;
            }
            case REQ_CELOWNIK: {
                println("Received REQ_CELOWNIK from %d time %d", status.MPI_SOURCE,packet.lamportTime)
                lockStateMutex();
                queueItem newItem{status.MPI_SOURCE, packet.lamportTime, packet.hasCelownik, packet.hasAgrafka};
                processData.addToVector(processData.celownikReqQueue, newItem);
                sendPacket(status.MPI_SOURCE, ACK_CELOWNIK);
                unlockStateMutex();
                break;
            }
            case ACK_AGRAFKA: {
                println("Received ACK_AGRAFKA from %d time %d", status.MPI_SOURCE,packet.lamportTime)
                lockStateMutex();
                queueItem newItem{status.MPI_SOURCE, packet.lamportTime, packet.hasCelownik, packet.hasAgrafka};
                processData.addToVector(processData.agrafkaAck, newItem);
                if (processData.canIHaveAgrafka()) {
                    println("I can have AGRAFKA")
                    processData.hasAgrafka = true;
                    processData.state = WAITING_CELOWNIK;
                    unlockStateMutex();
                    condVarNotify();
                }
                unlockStateMutex();
                break;
            }
            case ACK_CELOWNIK: {
                println("Received ACK_CELOWNIK from %d time %d", status.MPI_SOURCE,packet.lamportTime)
                lockStateMutex();
                queueItem newItem{status.MPI_SOURCE, packet.lamportTime, packet.hasCelownik, packet.hasAgrafka};
                processData.addToVector(processData.celownikAck , newItem);
                if (processData.canIHaveCelownik()) {
                    println("I can have CELOWNIK")
                    processData.hasCelownik = true;
                    processData.state = MAKING_BRON;
                    unlockStateMutex();
                    condVarNotify();
                }
                unlockStateMutex();
                break;
            }
            case RELEASE:{
                println("Received RELEASE from %d time %d", status.MPI_SOURCE,packet.lamportTime)
                lockStateMutex();
                processData.removeFromVector(processData.agrafkaReqQueue,status.MPI_SOURCE);
                processData.removeFromVector(processData.celownikReqQueue,status.MPI_SOURCE);
                if (status.MPI_SOURCE == processData.rank){
                    processData.hasCelownik = false;
                    processData.hasAgrafka = false;
                    processData.state = WAITING_AGRAFKA;
                    condVarNotify();
                }
                unlockStateMutex();
                break;
            }
        }
    }
}