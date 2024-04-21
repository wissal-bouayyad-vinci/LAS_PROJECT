#include "game.h"

char* initGrille(){
    char grille[GRILLE_SIZE];

    for (int i = 0; i < GRILLE_SIZE; ++i)
    {
        grille[i] = '-';
    }

    return grille;
}

bool choosePlacement(char tileNumber, int chosenPlace,char* grille){

    int placeNumber = chosenPlace-1;

    if (placeNumber < 0 || placeNumber >= GRILLE_SIZE ) {
        printf("Veuillez choisir une place viable.");
        return false;
    }

    while (grille[placeNumber] != '-'){
        placeNumber++;

        if (placeNumber == (GRILLE_SIZE-1)) {
            placeNumber=0;
        }
    }

    grille[placeNumber]=tileNumber;
    return true;
}



int scoreCalculation(char* grille){
    int points = 0;
    int suite_lenght =1;


    for (int i = 1; i < GRILLE_SIZE; ++i){

        if (grille[i]>grille[i-1])
        {
            suite_lenght++;
        }else{
            points+=TablePoints[suite_lenght];
            suite_lenght=1;
        }
    }

    points+=TablePoints[suite_lenght];
    return points;
}

