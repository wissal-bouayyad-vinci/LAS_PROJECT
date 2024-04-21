#include "game.h"

char* initGrille(int size){
    char grille[size];

    for (int i = 0; i < size; ++i)
    {
        grille[i] = '-1';
    }

    return grille;
}

bool chosePlacement(int tileNumber, int chosenPlace,char* grille,int size){
    while (grille[chosenPlace]!= '-1')
    {
        chosenPlace++;     

    }



}