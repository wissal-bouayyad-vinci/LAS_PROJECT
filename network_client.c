#include "network_client.h"
<<<<<<< HEAD
=======
#include "config.h"
>>>>>>> 48cfc972f06b663f7c186b1e372c782273df7bd9
#include "utils_v1.h"

int initSocketClient(char * serverIP, int serverPort){
    int sockfd = ssocket();
    sconnect(serverIP, serverPort, sockfd);
    return sockfd;
}

