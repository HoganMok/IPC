#include "screenThread.h"
#include "socket.h"

pthread_t threadScreen;
static pthread_cond_t s_syncOkToOutputCondVar = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t s_syncOkToOutputMutex = PTHREAD_MUTEX_INITIALIZER;
char* msg = "Received: ";

void* screenThread(void* unused){
    while (1) {
        pthread_mutex_lock(&s_syncOkToOutputMutex);
        {
            pthread_cond_wait(&s_syncOkToOutputCondVar, &s_syncOkToOutputMutex);
        }pthread_mutex_unlock(&s_syncOkToOutputMutex);
        char* message = Socket_list_remove();
        if (message != NULL) {    
            int strLength = strlen(message);
            write(STDOUT_FILENO, msg, strlen(msg));
            write(STDOUT_FILENO, message, strLength);
            if (strLength == 2 && *message == '!'){ 
                Socket_signalToShutdown();
                free(message);
                message = NULL;
                break;
                
            }
        }
        free(message);
        message = NULL;
    }
    return NULL;
}

void ScreenThread_init(){
    pthread_create(&threadScreen, NULL, screenThread, NULL);
}

void ScreenThread_readyToOutput(){
    pthread_mutex_lock(&s_syncOkToOutputMutex);
    {
        pthread_cond_signal(&s_syncOkToOutputCondVar);
    }pthread_mutex_unlock(&s_syncOkToOutputMutex);
}

void ScreenThread_waitForShutDown(){
    pthread_cancel(threadScreen);
    pthread_join(threadScreen, NULL);
    pthread_cond_destroy(&s_syncOkToOutputCondVar);
    pthread_mutex_destroy(&s_syncOkToOutputMutex);
}