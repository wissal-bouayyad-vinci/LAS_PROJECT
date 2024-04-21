#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#include "utils_v1.h"
#include "network_serveur.h"

int initSocketServer(int port)
{
    int sockfd = ssocket();

    // setsockopt -> to avoid Address Already in Use
    int option = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int));

    sbind(port, sockfd);

    slisten(sockfd, BACKLOG);

    return sockfd;
}