#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <pthread.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "list.h"
#include "keyBoardThread.h"
#include "senderThread.h"
#include "receiverThread.h"
#include "screenThread.h"
#define MAX_LEN 1024



void Socket_init(char** args);
void Socket_signalToShutdown();
void Socket_shutdown();
int Socket_list_append(void* pItem);
char* Socket_list_remove();

#endif