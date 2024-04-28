#include "network_client.h"
#include "utils_v1.h"

int initSocketClient(char *serverIP, int serverPort){
    int sockfd = ssocket();
    sconnect(serverIP, serverPort, sockfd);
    return sockfd;
}

