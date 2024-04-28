#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
 
#include "utils_v1.h"
#include "network_serveur.h"
#include "config.h"


int initSocketServer()
{
    int sockfd = ssocket();

    // setsockopt -> to avoid Address Already in Use
	// to do before bind !
	int option = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int));
    
    sbind(SERVER_PORT, sockfd);
    slisten(sockfd, BACKLOG);
 
    return sockfd;
}