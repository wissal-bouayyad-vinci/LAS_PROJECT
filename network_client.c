#include "network_client.h"

int initSocketClient(char * serverIP, int serverPort){
    int sockfd = ssocket();
    sconnect(serverIP, serverPort, sockfd);
    return sockfd;
}


int getSocketServer() {
    int server_socket;
    server_socket = ssocket();
    sbind(SERVER_PORT, server_socket);
    slisten(server_socket);
    return server_socket;
}
