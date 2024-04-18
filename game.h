#ifndef GAME_H
#define GAME_H 

#define BOARDGAME_SIZE 20

/**
 * PRE: tileNumber is number sent by the server for the player
 *      chosenPlace is the place in the table chosen by the player
 * POST: if the the chosenPlace is free ,the tileNumber will be put
 *       if not, then  tile is placed to the right of the 
 *       requested position.
 * RES:  once the tile is placed true will be returned
 */
bool chosePlacement(int tileNumber, int chosenPlace);

/**
 * PRE: grille is the final grille of the player
 * POST: it will calculate the final score of the player
 * RES: return the final score
 */
int scoreCalculation(int* grille);


/**
 * PRE: size of the boardgmae (20)
 * POST: it will create a table of size size
 * RES: return a int table
 */ 
int* initBoardGame(int size);


#endif
