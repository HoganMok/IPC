#include "receiverThread.h"
#include "socket.h"

pthread_t threadReceiver;
int rxDescriptor;

void* receiverThread(void* res){
    while (1){
        char messageRx[MAX_LEN];
        struct addrinfo *result = (struct addrinfo *)res;
        int bytesRx = recvfrom(rxDescriptor, messageRx, MAX_LEN, 0, result->ai_addr, &result->ai_addrlen);
        int terminateIdx = (bytesRx < MAX_LEN)? bytesRx : MAX_LEN -1;
        messageRx[terminateIdx] = 0;
        char* dynamicMessage = (char*)malloc(MAX_LEN);
        strcpy(dynamicMessage, messageRx);
        Socket_list_append(dynamicMessage);
        ScreenThread_readyToOutput();
    }
    return NULL;
}

void ReceiverThread_init(struct addrinfo *res, int socketDescriptor){
    rxDescriptor = socketDescriptor;
    pthread_create(&threadReceiver, NULL, receiverThread, res);
}

void ReceiverThread_waitForShutDown(){
    pthread_cancel(threadReceiver);
    pthread_join(threadReceiver, NULL);
    
}