#ifndef CLIENT_H
#define CLIENT_H

#define MAX_NUMBER_TURN 20
#include "player.h"

/* PRE:  grille :  is the int table with the tiles that will be printed.
 */
void printGrille(int *grille);

/**
 * PRE: players : is a table of players organized by their score.
 *      size :  is the size of the table (number of players).
 *      the ranking will be printed.
 */
void printRanking(Structplayer *players, int size);

/**
 * PRE: table is a table of  strings .
 * RES: return the number of lines from the table.
 */
size_t getTableSize(char **table);

#endif
