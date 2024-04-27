#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define GRILLE_SIZE 20

int TablePoints[] = {0, 1, 3, 5, 7, 9, 11, 15, 20, 25, 30, 35, 40, 50, 60, 70, 85, 100, 150, 300};
int scoreCalculation(int* grille){
    int points = 0;
    int sequence_length =1;
    int BJKER = 0;
    int AFJKER = 0;        
    int index = 0;
   
    for (int i = 1; i < GRILLE_SIZE; ++i){
        if (grille[i]==31)
        {
            index = i;
            BJKER= grille[i-1];
            AFJKER = grille[i+1];
        }        
        // si i > i-1 alors on est dans un sequence : 
        if (grille[i]>=grille[i-1] ){

            // Bjker !=0 veut dire que l'on a rencontrer un joker et qu'on a retenu la veleur avant et après celui celui ci
            if (BJKER!=0){
                // si apres joker >= avant joker alors on fais toujour partie d'une seul sequance
                if (AFJKER>=BJKER){
                    sequence_length++;
                }
                    // sinon on a deux serie un avant et une apres 
                else  {
                    // on calcul le before et le after longeur des serie pour comprarer
                    // on veut que la plus longue recupère le joker en tant que nombre +1 pour la longueur
                    int before=1;
                    int after=1;
                    for (int j = index-1; j > 0; j--) {
                        if (grille[j] >= grille[j - 1]) {
                            before++;
                        } else {
                            break;
                        }
                    }
                    for (int k = index+1; k < GRILLE_SIZE; ++k) {
                       
                        if (grille[k] <= grille[k+1]) {
                            after++;
                        } else {
                            break;
                        }
                    }

                    // si before avant alors on rajoute le +1 pour le joker et sinon on l efera dans la longeur après
                    if(before >= after){
                        before++;
                        points += TablePoints[before];
                        sequence_length=1;
                
                    }else{
                        points += TablePoints[before];
                        sequence_length=1;

                    }

                    // re initialiser les attributs
                    BJKER=0;
                    AFJKER=0;
                    index=0;
                    before=1;
                    after=1;

                }
       
            } else {
                sequence_length++;
            }

        }else{
            // on a fini la sequance donc on rentre les points et on recommence 
            points+=TablePoints[sequence_length-1];
            sequence_length=1;
        }
    }
    // pour rajouter les points de la toute dernière sequance.
    points+=TablePoints[sequence_length-1];
    return points;
}

int main(int argc, char const *argv[]){


    int grille[GRILLE_SIZE]={1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 31, 20};

    int points = scoreCalculation(grille);
    printf("%d\n",points );
    return 0;



}