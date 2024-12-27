#include <stdio.h>
#include <unistd.h> //read and write
#include <stdlib.h>
#include <netdb.h>  //socket
#include <string.h>

#include "list.h"
#include "socket.h"

#define MAX_LEN 1024

struct sockaddr_in sinn;
struct sockaddr_in sinRemote;

int main(int argCount, char** args){
    if (argCount != 4){
        printf("Error, please enter your port number, remote machine name, and remote port number\n");
        exit(EXIT_FAILURE);
    }

    Socket_init(args);
    //Socket_shutdown();
    return 0;
}