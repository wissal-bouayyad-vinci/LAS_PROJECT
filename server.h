#ifndef SERVER_H
#define SERVER_H

#define MAX_PLAYERS 3
#define TIME_INSCRIPTION 30
#define NUMBER_OF_PLAYS 20
#define NUMBER_OF_TILES 40
#define SCORE_TABLE_SIZE 3



/**
 * PRE:  serverPort: a valid port number
 * POST: on success, binds a socket to 0.0.0.0:serverPort and listens to it ;
 *       on failure, displays error cause and quits the program
 * RES:  return socket file descriptor
 */
int initSocketServer(int port)

/**
 * POST: on success creates a random displayed table containing all the necessary tiles displayed randomly.
 * RES: returns a map of tuiles
 */
int* createTiles();



void makeRanking()

#endif