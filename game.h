#ifndef GAME_H
#define GAME_H 


#include <stdbool.h>
#define GRILLE_SIZE 20



/**
 * PRE: tileNumber :  is number sent by the server for the player
 *      chosenPlace :  is the place in the table chosen by the player
 *      grille : where we put the  given tile.
 *      size : of the grille
 * POST: if the the chosenPlace is free ,the tileNumber will be put
 *       if not, then  tile is placed to the right of the 
 *       requested position.
 * RES:  once the tile is placed true will be returned
 */
bool choosePlacement(int tileNumber, int chosenPlace,int* grille);

/**
 * PRE: grille :  is the final grille of the player
 *      
 * POST: it will calculate the final score of the player
 * RES: return the final score
 */
int scoreCalculation(int* grille);


/**
 * 
 * POST: it will create a table of size GRILLE_SIZE
 * RES: return a int* table
 */ 
int* initGrille();


#endif
