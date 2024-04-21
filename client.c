#include "client.h"
#include "game.h"

void printGrille(char* grille, int size){

    printf("GRILLE DE JEUX\n");

    for (int i = 0; i < size; ++i)
    {
        printf("case n°%s : %d  |\n  ", (i+1),grille[i]);
    }
}


void printRanking(struct Structplayer* players,int size){

    printf("RANKING FINAL\n");

    for (int i = 0; i < size; ++i)
    {
        printf("%d. %s\n",(i+1),players[i].pseudo);
    }

}

