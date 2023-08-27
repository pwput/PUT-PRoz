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
                println("MAKING_BRON_AND_KILLING_RAT");
                lockStateMutex();
                sleep(MAKING_AND_KILLING_TIME);
                sendPacketToAll(RELEASE);
                //processData.state=WAITING_CELOWNIK;
                processData.hasAgrafka=true;
                processData.hasCelownik=true;
                unlockStateMutex();
                condVarWait();
                break;
            }
            default:
                println("UNSUPORTEDD State");
        }
    }
            sleep(1 + rand() % 10);
}
