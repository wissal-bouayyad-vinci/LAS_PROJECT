#include "game.h"

char* initGrille(int size){
    char grille[size];

    for (int i = 0; i < size; ++i)
    {
        grille[i] = '-';
    }

    return grille;
}

bool chosePlacement(int tileNumber, int chosenPlace,char* grille,int size){
    if (chosenPlace < 0 || chosenPlace >= size ) {
        printf("Veuillez choisir une place viable.");
        return false;
    }

    while (chosenPlace < size && grille[chosenPlace] != '-'){
        chosenPlace++;
    }

    if (chosenPlace >= size) {
        printf("Veuillez choisir une autre place.");
        return false;
    }

    grille[chosenPlace]=tileNumber+'0';
    return true;
}



int scoreCalculation(char* grille,int size){
    int points = 0;
    int suite_lenght =0;


    for (int i = 0; i < size; ++i)
    {
        if (grille[i]>grille[i-1])
        {
            suite_lenght++;
        }else{
            points+=TablePoints[suite_lenght];
            suite_lenght=0;
        }
    }

    points+=TablePoints[suite_lenght];
    return points;
}

