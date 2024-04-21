#ifndef CLIENT_H
#define CLIENT_H


/* PRE:  grille is the int table with the tile
 *      size is the size of the grille (20)
 *      grille is the  grille of the player
 * POST: it will print the grille of the player
 */
void printGrille(char* grille,int size);

/**
 * PRE: player is a table of player organized by their score
 *      size is the size of the table (number of players)
 * POST:it will print the ranking
 */ 
void printRanking(struct Structplayer* players,int size);


#endif

