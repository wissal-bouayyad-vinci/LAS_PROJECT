#ifndef SERVER_H
#define SERVER_H

#define MAX_PLAYERS 3
#define TIME_INSCRIPTION 30
#define NUMBER_OF_PLAYS 20
#define NUMBER_OF_TUILES 40
#define SCORE_TABLE_SIZE 3


//Structur epour une tuile
typedef struct {
    int number;
}Tile;



/**
 * PRE:  serverPort: a valid port number
 * POST: on success, binds a socket to 0.0.0.0:serverPort and listens to it ;
 *       on failure, displays error cause and quits the program
 * RES:  return socket file descriptor
 */
int initSocketServer(int port)

/**
 * RES: returns a map of tuiles
 */
struct mapTuiles createTuiles()

/**
 * PRE: the map of tuiles still available
 * POST: the map of tuiles without the one chosen randomly
 * RES: the randomly chosen tuile
 */
struct Tuile takeRandom()

void makeRanking()

#endif