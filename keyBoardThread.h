#ifndef _KEYBOARDTHREAD_H_
#define _KEYBOARDTHREAD_H_

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "list.h"
#include "senderThread.h"

void KeyBoardThread_init();
void KeyBoardThread_waitForShutDown();

#endif