#ifndef GLOBALH
#define GLOBALH
#pragma once

#include "data/processData.h"
#include "threads/main_thread.h"
#include <mpi.h>
#include <pthread.h>

#define println(FORMAT, ...) printf("%c[%d;%dm[r:%d,t:%d,p:%c]: " FORMAT "%c[%d;%dm\n",  27, (1+(processData.rank/7))%2, 31+(6+processData.rank)%7, \
processData.rank, processData.lamportTime, processData.getProcessTypeLetter(), ##__VA_ARGS__, 27,0,37);


extern ProcessData processData;

extern pthread_mutex_t lamportMutex;
extern pthread_mutex_t stateMutex;
extern pthread_cond_t cond;
extern pthread_mutex_t condMutex;

extern MPI_Datatype MPI_PACKET_T;

void init(int *argc, char ***argv);
void initMessageType();
void check_thread_support(int provided);
void finalize();
void initCommunicationThread(); //inicjalizuje watek komunikacyjny i typ

void lockStateMutex();
void unlockStateMutex();
int condVarWait() ;
int condVarNotify() ;

#endif