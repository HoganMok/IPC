#include "socket.h"

static int socketDescriptor;
static struct addrinfo *res;
List* list;
static pthread_cond_t s_syncOkToShutdownCondVar = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t s_syncOkToShutdownMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t s_syncOkToAppendMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t s_syncOkToRemoveMutex = PTHREAD_MUTEX_INITIALIZER;

void Socket_init(char** args){
    printf("Starting......\n");
    list = List_create();
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(atoi(args[1]));
    socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (bind (socketDescriptor, (struct sockaddr*)&sin, sizeof(sin)) < 0){
        printf("Error, socket can not be binded\n");
        return;
    }

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_DGRAM; // Datagram socket

    int s = getaddrinfo(args[2], args[3], &hints, &res);

    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }
    KeyBoardThread_init();
    SenderThread_init(res, socketDescriptor);
    ReceiverThread_init(res, socketDescriptor);
    ScreenThread_init();
    
    pthread_mutex_lock(&s_syncOkToShutdownMutex);
    {
        pthread_cond_wait(&s_syncOkToShutdownCondVar, &s_syncOkToShutdownMutex);
    }pthread_mutex_unlock(&s_syncOkToShutdownMutex);
    Socket_shutdown();
}

void freeNodeData(void* pItem){
    free(pItem);
    pItem = NULL;
}

void Socket_signalToShutdown(){
    pthread_mutex_lock(&s_syncOkToShutdownMutex);
    {
        pthread_cond_signal(&s_syncOkToShutdownCondVar);
    }
    pthread_mutex_unlock(&s_syncOkToShutdownMutex);

}

void Socket_shutdown(){
    if (List_count(list) != 0) List_free(list, freeNodeData);
    
    KeyBoardThread_waitForShutDown();
    
    SenderThread_waitForShutDown();
    
    ReceiverThread_waitForShutDown();
    
    ScreenThread_waitForShutDown();

    pthread_cond_destroy(&s_syncOkToShutdownCondVar);
    pthread_mutex_destroy(&s_syncOkToShutdownMutex);
    pthread_mutex_destroy(&s_syncOkToRemoveMutex);
    pthread_mutex_destroy(&s_syncOkToAppendMutex);

    free(res);
    res = NULL;
    close(socketDescriptor);
    printf("Terminated\n");
}

int Socket_list_append(void* pItem){
    pthread_mutex_lock(&s_syncOkToAppendMutex);
    {
        if (List_append(list, pItem) == -1) {
            printf("Error, List is full\n");
            Socket_signalToShutdown();
            return -1;
        }
    }
    pthread_mutex_unlock(&s_syncOkToAppendMutex);
    return 0;
}

char* Socket_list_remove(){
    char* msg;
    pthread_mutex_lock(&s_syncOkToRemoveMutex);
    {
        msg = (char*)(List_remove(list));
    }
    pthread_mutex_unlock(&s_syncOkToRemoveMutex);
    return msg;
}