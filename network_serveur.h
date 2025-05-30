#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H
 

 
/**
* PRE:  port: the given port 
* POST: on success, binds a socket to 0.0.0.0:serverPort and listens to it ;
*       on failure, displays error cause and quits the program
* RES:  return socket file descriptor
*/
int initSocketServer(int port);
 
#endif