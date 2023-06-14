#include "main.h"
#include "threads/communication_thread.cpp"
#include "data/processData.cpp"
#include "threads/main_thread.cpp"
#include "utils.h"
#include "utils.cpp"

MPI_Datatype MPI_PACKET_T;

ProcessData processData;

pthread_t communicationTread;

pthread_mutex_t lamportMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t stateMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t condMutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv) {
    init(&argc, &argv);
    mainLoop();
    finalize();
    return 0;
}

void init(int *argc, char ***argv){
    println("App initialization start");

    int provided;

    MPI_Init_thread(argc, argv, MPI_THREAD_MULTIPLE, &provided);
    check_thread_support(provided);

    initMessageType();

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(NULL) * rank);


    processData.init(rank, size);
    initCommunicationThread();

    println("App initialization complete");
}

void initMessageType(){
    const int nItems = 3;
    int blockLengths[nItems] = { 1, 1, 1};
    MPI_Datatype types[nItems] = { MPI_INT, MPI_C_BOOL, MPI_C_BOOL };

    MPI_Aint offsets[nItems];
    offsets[0] = offsetof(packet_t, lamportTime);
    offsets[1] = offsetof(packet_t, hasAgrafka);
    offsets[2] = offsetof(packet_t, hasCelownik);

    MPI_Type_create_struct(nItems, blockLengths, offsets, types, &MPI_PACKET_T);
    MPI_Type_commit(&MPI_PACKET_T);
};

void initCommunicationThread(){
    switch (processData.processType) {
        case GNOM:
            pthread_create( &communicationTread, NULL, communicationLoopGnom , 0);
            break;
        case SKRZAT:
            pthread_create( &communicationTread, NULL, communicationLoopSkrzat , 0);
            break;
    }
}

void check_thread_support(int provided) {
    if (DEBUG) printf("THREAD SUPPORT: requested %d\n", provided);
    switch (provided) {
        case MPI_THREAD_SINGLE:
            fprintf(stderr, "No tread support - leaving...\n");
            MPI_Finalize();
            exit(-1);
            break;
        case MPI_THREAD_FUNNELED:
            if (DEBUG) printf("MPI_THREAD_FUNNELED\n");
            break;
        case MPI_THREAD_SERIALIZED:
            if (DEBUG) printf("MPI_THREAD_SERIALIZED\n");
            break;
        case MPI_THREAD_MULTIPLE:
            if (DEBUG) printf("Full thread support.\n");
            break;
        default:
            if (DEBUG) printf("No information returned...\n");
    }
}

void finalize(){
    pthread_join(communicationTread, NULL);
    MPI_Type_free(&MPI_PACKET_T);
    MPI_Finalize();
}
void lockStateMutex(){
    pthread_mutex_lock(&stateMutex);
};
void unlockStateMutex(){
    pthread_mutex_unlock(&stateMutex);
}

int condVarWait() {
    return pthread_cond_wait(&cond, &condMutex);
}

int condVarNotify() {
    return pthread_cond_signal(&cond);
}
