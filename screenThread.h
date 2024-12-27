#ifndef _SCREENTHREAD_H_
#define _SCREENTHREAD_H_

#include <pthread.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "list.h"

void ScreenThread_init();
void ScreenThread_readyToOutput();
void ScreenThread_waitForShutDown();

#endif