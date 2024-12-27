#ifndef _RECEIVERTHREAD_H_
#define _RECEIVERTHREAD_H_

#include <pthread.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "list.h"
#include "screenThread.h"

void ReceiverThread_init(struct addrinfo *res, int socketDescriptor);
void ReceiverThread_waitForShutDown();

#endif