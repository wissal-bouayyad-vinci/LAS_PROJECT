#include "game.h"

int* initGrille(){
    char* grille[GRILLE_SIZE];

    for (int i = 0; i < GRILLE_SIZE; ++i)
    {
        grille[i] = '-';
    }

    return grille;
}

bool choosePlacement(char* tileNumber, int chosenPlace,int* grille){

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


//lolololol

int scoreCalculation(int* grille){
    int points = 0;
    int suite_lenght =1;

    int BJKER = 0;        

    for (int i = 1; i < GRILLE_SIZE; ++i){

        if (BJKER!=0)
        {
            if (grille[i]>BJKER)
            {
                suite_lenght++:
            }else{
            points+=TablePoints[suite_lenght];
            suite_lenght=1;

        }
        
        BJKER=0;
        }
        if (grille[i]>grille[i-1] ){
            if (grille[i]==31)
            {
                BJKER==grille[i-1];
            }
        
            suite_lenght++;
        
        }else{
            points+=TablePoints[suite_lenght];
            suite_lenght=1;
        }
    }

    points+=TablePoints[suite_lenght];
    return points;
}

