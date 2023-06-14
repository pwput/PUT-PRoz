//
// Created by pwiet on 16.05.2023.
//

#ifndef UNTITLED1_UTILS_H
#define UNTITLED1_UTILS_H

#include <string.h>
#include <string>
#include "data/processData.h"
#include <vector>


void printVector(std::vector<queueItem> vec);
void logDebug(std::string message);
void log(std::string message);

/* wysyłanie pakietu, skrót: wskaźnik do pakietu (0 oznacza stwórz pusty pakiet), do kogo, z jakim typem */
void sendPacket(int destination, int tag);
void sendPacketToAll(int tag , ProcessType type);

#endif //UNTITLED1_UTILS_H
