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
        println("Received %s from r:%d (t:%d, ha:%d, hc:%d)", MessageText[status.MPI_TAG].c_str(),status.MPI_SOURCE,packet.lamportTime,packet.hasAgrafka,packet.hasCelownik)
        switch (status.MPI_TAG) {
            case REQ_AGRAFKA: {
                 bool cont=true;
                while(cont){
                cont=false;
                lockStateMutex();
                queueItem newItem{status.MPI_SOURCE, packet.lamportTime, packet.hasCelownik, packet.hasAgrafka};
                processData.addToVector(processData.agrafkaReqQueue, newItem);
                sendPacket(status.MPI_SOURCE, ACK_AGRAFKA);
                }
                unlockStateMutex();
                break;
            }
            case REQ_CELOWNIK: {
                bool cont=true;
                while(cont){
                lockStateMutex();
                cont=false;
                queueItem newItem{status.MPI_SOURCE, packet.lamportTime, packet.hasCelownik, packet.hasAgrafka};
                processData.addToVector(processData.celownikReqQueue, newItem);
                sendPacket(status.MPI_SOURCE, ACK_CELOWNIK);
                unlockStateMutex();
                }
                break;
            }
            case ACK_AGRAFKA: {
                bool cont=true;
                while(cont){
                lockStateMutex();
                cont=false;
                queueItem newItem{status.MPI_SOURCE, packet.lamportTime, packet.hasCelownik, packet.hasAgrafka};
                processData.addToVector(processData.agrafkaAck, newItem);
                if (processData.canIHaveAgrafka()) {
              //      println("I can have AGRAFKA")
                    processData.hasAgrafka = true;
                    processData.state = WAITING_CELOWNIK;
                    unlockStateMutex();
                    condVarNotify();
                }
                }
                unlockStateMutex();
                break;
            }
            case ACK_CELOWNIK: {
            bool contin=true;
            while(contin){
                lockStateMutex();
                contin=false;
                queueItem newItem{status.MPI_SOURCE, packet.lamportTime, packet.hasCelownik, packet.hasAgrafka};
                processData.addToVector(processData.celownikAck , newItem);
                if (processData.canIHaveCelownik()) {
             //       println("I can have CELOWNIK")
                    processData.hasCelownik = true;
                    processData.state = MAKING_BRON;
                    unlockStateMutex();
                    condVarNotify();
                }
                }
                unlockStateMutex();
                break;
            }
            case RELEASE:{
                bool contin=true;
                while (contin){
                lockStateMutex();
                contin=false;
                processData.removeFromVector(processData.agrafkaReqQueue,status.MPI_SOURCE);
                processData.removeFromVector(processData.celownikReqQueue,status.MPI_SOURCE);
                if (status.MPI_SOURCE == processData.rank){
                    processData.hasCelownik = false;
                    processData.hasAgrafka = false;
                    processData.state = WAITING_AGRAFKA;
                    condVarNotify();
                }
                if (processData.canIHaveAgrafka()) {
                  //  println("I can have AGRAFKA")
                    processData.hasAgrafka = true;
                    processData.state = WAITING_CELOWNIK;
                    condVarNotify();
                }
                if (processData.canIHaveCelownik()) {
                  //  println("I can have CELOWNIK")
                    processData.hasCelownik = true;
                    processData.state = MAKING_BRON;
                    condVarNotify();
                }
                }
                unlockStateMutex();
                break;
            }
        }
    }
}