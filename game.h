#ifndef GAME_H
#define GAME_H 

#define GRILLE_SIZE 20

int TablePoints[] = {0, 1, 3, 5, 7, 9, 11, 15, 20, 25, 30, 35, 40, 50, 60, 70, 85, 100, 150, 300};


/**
 * PRE: tileNumber is number sent by the server for the player
 *      chosenPlace is the place in the table chosen by the player
 *      grille dans laquel se poser le tileNumber
 *      size of the grille
 * POST: if the the chosenPlace is free ,the tileNumber will be put
 *       if not, then  tile is placed to the right of the 
 *       requested position.
 * RES:  once the tile is placed true will be returned
 */
bool choosePlacement(char* tileNumber, int chosenPlace,char* grille);

/**
 * PRE: grille is the final grille of the player
 *      size id the size of the grille
 * POST: it will calculate the final score of the player
 * RES: return the final score
 */
int scoreCalculation(char* grille);


/**
 * PRE: size of the player grille (20)
 * POST: it will create a table of size size
 * RES: return a int table
 */ 
char* initGrille();


#endif
