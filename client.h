#ifndef CLIENT_H
#define CLIENT_H

#define MAX_NUMBER_TURN 20
#include "player.h"

/* PRE:  grille is the int table with the tile
 *      size is the size of the grille (20)
 *      grille is the  grille of the player
 * POST: it will print the grille of the player
 */
void printGrille(int* grille);

/**
 * PRE: player is a table of player organized by their score
 *      size is the size of the table (number of players)
 * POST:it will print the ranking
 */ 
void printRanking( Structplayer* players,int size);

 
/**
 * PRE: table is a table of  strings 
 * RES:the number of lines from the table
 */
 size_t getTableSize(char **table);

#endif

