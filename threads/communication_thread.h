//
// Created by pwiet on 16.05.2023.
//

#ifndef UNTITLED1_COMMUNICATION_THREAD_H
#define UNTITLED1_COMMUNICATION_THREAD_H

#include "../common.h"

void *communicationLoop(void *ptr);


void communicationLoopGnom(MPI_Status status,packet_t packet);
void communicationLoopSkrzat(MPI_Status status,packet_t packet);

#endif //UNTITLED1_COMMUNICATION_THREAD_H
