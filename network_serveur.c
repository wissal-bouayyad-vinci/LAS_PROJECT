#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
 
#include "utils_v1.h"
#include "network_serveur.h"
#include "config.h"
<<<<<<< HEAD


=======
 
 
>>>>>>> 48cfc972f06b663f7c186b1e372c782273df7bd9
int initSocketServer(int port)
{
    int sockfd = ssocket();
    sbind(port, sockfd);
    slisten(sockfd, BACKLOG);
 
    return sockfd;
}