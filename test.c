#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "utils_v1.h"


int main(int argc, char const *argv[])
{
    printf("Test de la methode tilebag");

    char* tilebag [40];
    int index;
    char* available [] = {
        "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
        "11", "11", "12", "12", "13", "13", "14", "14",
        "15", "15", "16", "16", "17", "17", "18", "18",
        "19", "19", "20", "21", "22", "23", "24", "25", "26",
        "27", "28", "29", "30", "J"
    };

    // Initialize tilebag with NULL pointers
    for (int i = 0; i < 40; i++) {
        tilebag[i] = NULL;
    }

    for(int i=0 ; i<40 ; i++) {
        index = randomIntBetween(0,39);
        while(tilebag[index]!=NULL) {
            index++;
            if(index == 40){
                index=0;
            }
        }
        tilebag[index] = available[i];  
    }

    for  (int i = 0; i<40;i++){
        printf("%s\n",tilebag[i]);
    }

    exit(0);
}