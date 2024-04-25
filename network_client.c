#include "network_client.h"
#include "config.h"

int initSocketClient(char * serverIP, int serverPort){
    int sockfd = ssocket();
    sconnect(serverIP, serverPort, sockfd);
    return sockfd;
}

