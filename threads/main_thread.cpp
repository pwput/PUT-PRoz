//
// Created by pwiet on 16.05.2023.
//

#include "main_thread.h"
#include "../main.h"
#include "../utils.h"


void mainLoop(){
    switch (processData.processType) {
        case GNOM:
            mainLoopGnom();
        case SKRZAT:
            mainLoopSkrzat();

    }


}

void mainLoopGnom(){
    while (true){
        switch (processData.state) {
            case WAITING_CELOWNIK:
                println("WAITING_CELOWNIK");
                sendPacketToAll(REQ_CELOWNIK);
                condVarWait();
                break;
            case WAITING_AGRAFKA:
                println("WAITING_AGRAFKA");
                sendPacketToAll(REQ_AGRAFKA);
                condVarWait();
                break;
            case MAKING_BRON:
                println("MAKING_BRON");
                break;
            default:
                println("UNSUPORTEDD State");
        }
    }
}

void mainLoopSkrzat(){
    while (true){
        switch (processData.state) {
            case KILLING:
                println("KILLING");
                break;
            case WAITING_BRON:
                println("WAITING_BRON");
                sendPacketToAll(REQ_BRON);
                condVarWait();
                break;
            default:
                println("UNSUPORTEDD State");
        }
    }
}