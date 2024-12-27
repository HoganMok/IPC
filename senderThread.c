#include "senderThread.h"
#include "socket.h"

static pthread_cond_t s_syncOkToSendCondVar = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t s_syncOkToSendMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t threadSender;
int descriptor;
char* sentMsg = "Sent: ";

void* senderThread(void* res){
    while (1){
        pthread_mutex_lock(&s_syncOkToSendMutex);
        {
            pthread_cond_wait(&s_syncOkToSendCondVar, &s_syncOkToSendMutex);
        }pthread_mutex_unlock(&s_syncOkToSendMutex);
        char* messageTo = Socket_list_remove();
        if (messageTo != NULL){
            struct addrinfo *result = (struct addrinfo *)res;
            if (sendto(descriptor, messageTo, strlen(messageTo), 0, result->ai_addr, result->ai_addrlen) == -1) {
                perror("Failed to Send\n");
                free(messageTo);
                messageTo = NULL;
                Socket_signalToShutdown();
                break;
            }
            else{
                write(STDOUT_FILENO, sentMsg, strlen(sentMsg));
                write(STDOUT_FILENO, messageTo, strlen(messageTo));
                if (strlen(messageTo) == 2 && messageTo[0] == '!'){ 
                    free(messageTo);
                    messageTo = NULL;
                    Socket_signalToShutdown();
                    break;
                }
            }
        }
        free(messageTo);
        messageTo = NULL;
    }
    return NULL;
}

void SenderThread_init(struct addrinfo *res, int socketDescriptor){
    descriptor = socketDescriptor;
    pthread_create(&threadSender, NULL, senderThread, res);
}

void Sender_readyToSend(){
    pthread_mutex_lock(&s_syncOkToSendMutex);
    {
        pthread_cond_signal(&s_syncOkToSendCondVar);
    }
    pthread_mutex_unlock(&s_syncOkToSendMutex);
}

void SenderThread_waitForShutDown(){
    pthread_cancel(threadSender);    
    pthread_join(threadSender, NULL);
    pthread_mutex_destroy(&s_syncOkToSendMutex);
    pthread_cond_destroy(&s_syncOkToSendCondVar);
}