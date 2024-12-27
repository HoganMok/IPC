#include "keyBoardThread.h"
#include "socket.h"

pthread_t threadKeyBoard;

void* keyBoardThread(void* unused){
    while (1){
        fflush(stdout);
        char messageRead[MAX_LEN];
        fgets(messageRead, MAX_LEN, stdin);
        int readLen = strlen(messageRead);

        if (readLen > 1) {
            char* dynamicMessage = (char*)malloc(MAX_LEN);
            strcpy(dynamicMessage, messageRead);
            if (Socket_list_append(dynamicMessage) == 0){
                Sender_readyToSend();
            }
        }
    }
    return NULL;
}

void KeyBoardThread_init(){
    pthread_create(&threadKeyBoard, NULL, keyBoardThread, NULL);
}

void KeyBoardThread_waitForShutDown(){
    pthread_cancel(threadKeyBoard);
    pthread_join(threadKeyBoard, NULL);
}