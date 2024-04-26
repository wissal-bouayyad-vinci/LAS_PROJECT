#include <stdio.h>
#include <stdlib.h>
#include "game.h"

int TablePoints[] = {0, 1, 3, 5, 7, 9, 11, 15, 20, 25, 30, 35, 40, 50, 60, 70, 85, 100, 150, 300};


int* initGrille(){
    int* grille = malloc(GRILLE_SIZE * sizeof(int));

    if (grille == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        // a demander au prof 
        exit(1);
    }

    for (int i = 0; i < GRILLE_SIZE; ++i)
    {
        grille[i] = -1;
    }

    return grille;
}

bool choosePlacement(int tileNumber, int chosenPlace,int* grille){

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



int scoreCalculation(int* grille){
    int points = 0;
    int sequence_length =1;
    int BJKER = 0;        

    for (int i = 1; i < GRILLE_SIZE; ++i){

        // Bjker !=0 veut dire que l'on a rencontrer un joker et qu'on a retenu la veleur avant celui celui ci
        if (BJKER!=0){
            // on compare le après joker avec le avant joker 
            // si c'est plus grand alors on est toujour dans une seule et même serie
            if (grille[i]>=BJKER){
                //donc on rajoute +1 a la longeur et oncontinue
                sequence_length++;
            }else{
                // on a deux serie un avant et une apres 
                // on calcul le before et le after longeur des serie 
                // on veut que la plus longue recupère le joker en tant que nombre +1 pour la longueur
                int before=1;
                int after=1;
                for(int j=(i-1) ; j>0;j--){
                    if(grille[j] >= grille[j-1]){
                        before++;
                    }else{
                        break;
                    }
                }
                for (int k = (i+1); k < GRILLE_SIZE; ++k){
                    if(grille[i+1] >=grille[i] ){
                        after++;
                    } else{
                        break;
                    }
                }

                // si before avant alors on rajoute le +1 dan sla serie avant le joker et sinon dans la longeur après
                if(before>=after){
                    sequence_length++;
                }else{
                    points+=TablePoints[sequence_length];
                    sequence_length=1;
                }
            }
            // on remet a joker non rencontrer
            BJKER=0;
        }

        // si i > i-1 alors 
        if (grille[i]>=grille[i-1] ){
            // est ce un joker ?
             if(grille[i]==31){
                 // c'est un joker 
                 BJKER = grille[i-1];
             }
        
        }else{
            points+=TablePoints[sequence_length];
            sequence_length=1;
        }
    }

    // gestion su dernier serie 
    points+=TablePoints[sequence_length];
    return points;
}


