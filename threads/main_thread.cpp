//
// Created by pwiet on 16.05.2023.
//

#include "main_thread.h"
#include "../main.h"
#include "../utils.h"
#include "../data/config.h"
#include <sys/wait.h>


void mainLoop() {
    while (true) {
        switch (processData.state) {
            case WAITING_CELOWNIK: {
                println("WAITING_CELOWNIK");
                lockStateMutex();
                processData.incLamportTime();
                sendPacketToAll(REQ_CELOWNIK);
                unlockStateMutex();
                condVarWait();
                break;
            }
            case WAITING_AGRAFKA: {
                println("WAITING_AGRAFKA");
                lockStateMutex();
                processData.incLamportTime();
                sendPacketToAll(REQ_AGRAFKA);
                unlockStateMutex();
                condVarWait();
                break;
            }
            case MAKING_BRON: {
                println("MAKING_BRON");
                lockStateMutex();
                sleep(MAKING_AND_KILLING_TIME);
                sendPacketToAll(RELEASE);
                unlockStateMutex();
                break;
            }
            default:
                println("UNSUPORTEDD State");
        }
    }
}

void mainLoopGnom() {
}

//void mainLoopSkrzat() {
//    while (true) {
//        switch (processData.state) {
//            case KILLING: {
//                println("KILLING");
//                lockStateMutex();
//                sleep(KILLING_TIME);
//                sendPacketToAll(RELEASE_AGRAFKA,ALL);
//                sendPacketToAll(RELEASE_CELOWNIK,ALL);
//                unlockStateMutex();
//                break;
//            }
//            case WAITING_BRON: {
//                println("WAITING_BRON");
//                sendPacketToAll(REQ_BRON,ALL);
//                condVarWait();
//                break;
//            }
//            default:
//                println("UNSUPORTEDD State");
//        }
//    }
//}