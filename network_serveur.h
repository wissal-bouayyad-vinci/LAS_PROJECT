#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H
 
//VARIABLES USED IN SERVER
#define MAX_PLAYERS 5
#define MIN_PLAYERS 2
 
/**
* PRE:  /
* POST: on success, binds a socket to 0.0.0.0:serverPort and listens to it ;
*       on failure, displays error cause and quits the program
* RES:  return socket file descriptor
*/
int initSocketServer();
 
#endif