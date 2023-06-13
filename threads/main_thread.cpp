//
// Created by pwiet on 16.05.2023.
//

#include "main_thread.h"
#include "../main.h"
#include "../utils.h"
#include "../data/config.h"
#include <sys/wait.h>


void mainLoop() {
    switch (processData.processType) {
        case GNOM:
            mainLoopGnom();
        case SKRZAT:
            mainLoopSkrzat();
    }
}

void mainLoopGnom() {
    while (true) {
        switch (processData.state) {
            case WAITING_CELOWNIK: {
                println("WAITING_CELOWNIK");
                lockStateMutex();
                processData.incLamportTime();
                auto newItem = queueItem{processData.rank, processData.lamportTime,processData.hasCelownik,processData.hasAgrafka};
                processData.addToVector(processData.celownikQueue, newItem);
                sendPacketToAll(REQ_CELOWNIK);
                unlockStateMutex();
                condVarWait();
                break;
            }
            case WAITING_AGRAFKA: {
                println("WAITING_AGRAFKA");
                lockStateMutex();
                processData.incLamportTime();
                auto newItem = processData.getNewQueueItem();
                processData.addToVector(processData.agrafkaQueue, newItem);
                sendPacketToAll(REQ_AGRAFKA);
                unlockStateMutex();
                condVarWait();
                break;
            }
            case MAKING_BRON: {
                println("MAKING_BRON");
                sleep(MAKING_TIME);
                sendPacketToAll(ACK_BRON);
                condVarWait();
                break;
            }
            default:
                println("UNSUPORTEDD State");
        }
    }

}

void mainLoopSkrzat() {
    while (true) {
        switch (processData.state) {
            case KILLING: {
                println("KILLING");
                lockStateMutex();
                sleep(KILLING_TIME);
                sendPacketToAll(RELEASE_AGRAFKA);
                sendPacketToAll(RELEASE_CELOWNIK);
                unlockStateMutex();

                break;
            }
            case WAITING_BRON: {
                println("WAITING_BRON");
                sendPacketToAll(REQ_BRON);
                condVarWait();
                break;
            }
            default:
                println("UNSUPORTEDD State");
        }
    }
}