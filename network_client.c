#include "network_client.h"

int initSocketClient(char * serverIP, int serverPort){
    int sockfd = ssocket();
    sconnect(serverIP, serverPort, sockfd);
    return sockfd;
}

