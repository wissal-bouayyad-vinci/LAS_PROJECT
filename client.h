#ifndef CLIENT
#define CLIENT 



/**

int* initSharedMemoryClient(int key);
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

