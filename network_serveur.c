#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#include "utils_v1.h"

int initSocketServer(int port)
{
    int sockfd = ssocket();

    /* no socket error */

    // setsockopt -> to avoid Address Already in Use
    // to do before bind !
    int option = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int));

    sbind(port, sockfd);

    /* no bind error */
    slisten(sockfd, BACKLOG);

    /* no listen error */
    return sockfd;
}