#ifndef CLIENT
#define CLIENT 


/**
 * PRE: serverIP : a valid IP address
 *      serverPort: a valid port number
 * POST: on success, connects a client socket to serverIP:serverPort
 *       on failure, displays error cause and quits the program
 * RES: return socket file descriptor
 */
int initSocketClient(char * serverIP, int serverPort);

/**
 * PRE: key is the key to create a shared memory.
 * POST: on success, you get the shared memory pointer.
 *       on failure, you get NULL.
 * RES:  return a pointer to the shared memory.
 */
int* initSharedMemoryClient(int key);


/**
 * PRE: key is the key to the existing shared memory.
 * 
 * POST: on success, you get the shared memory pointer.
 *       on failure, you get NULL.
 * RES:  return a pointer to the shared memory.
 */
int* getsharedMemoryClient(int key);

/**
 * PRE: tileNumber is number sent by the server for the player
 *      chosenPlace is the place in the table chosen by the player
 *      grille the final grille of the player 
 * POST: if the the chosenPlace is free ,the tileNumber will be put
 *       if not, then  tile is placed to the right of the 
 *       requested position.
 * RES:  once the tile is placed true will be returned
 */
bool chosePlacement(int tileNumber, int chosenPlace,int* grille);

/**
 * PRE: grille is the final grille of the player
 * POST: it will calculate the final score of the player
 * RES: return the final score
 */
int scoreCalculation(int* grille);

/**
 * PRE: grille is the final grille of the player
 * POST: it will print the grille of the player
 */
void printGrille(int* grille);

/**
 * PRE: player is a table of player organized by their score
 *      size is the size of the table (number of players)
 * POST: it will print the ranking
 */ 
void printRanking(Structplayer* player,int size);

#endif

