#ifndef _SENDERTHREAD_H_
#define _SENDERTHREAD_H_

#include <pthread.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "list.h"

void SenderThread_init(struct addrinfo *res, int socketDescriptor);
void Sender_readyToSend();
void SenderThread_waitForShutDown();

#endif