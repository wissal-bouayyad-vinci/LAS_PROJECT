#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>

#include "player.h"
#include "game.h"
#include "network_client.h"
#include "message.h"
#include "utils_v1.h"
#include "config.h"
#include "client.h"


void printGrille(int* grille, int size){

    printf("GRILLE DE JEUX\n");

    printf("%2s ", "");
    for (int i = 0; i < size; ++i)
    {
        printf("%3d ", (i+1));
    }
    printf("\n");

    printf("%2s ", "");
    for (int i = 0; i < size; ++i)
    {
        printf("%3d ",grille[i]);
    }
    printf("\n");
}


void printRanking( Structplayer* players,int size){

    printf("RANKING FINAL\n");

    for (int i = 0; i < size; ++i)
    {
        printf("%d. %s with a score of %d\n",(i+1),players[i].pseudo, players[i].score);
    }

}


int main(int argc, char const *argv[])
{
    // attributs : 
    char pseudoPlayer[MAX_PSEUDO];
    int sockfd;
    int ret;
    int tileNumber;
    int chosenPlacement;
    int cptGame=0;
    StructMessage message;

    // recup
    printf("Bienvenue a vous. Inscrivez vous pour commencer la partie.");
    printf("Pour cela votre pseudo : ");
    ret = sread(0,pseudoPlayer,MAX_PSEUDO);
    pseudoPlayer[ret - 1] = '\0';
    strcpy(message.messageText,pseudoPlayer);
    message.code = INSCRIPTION_REQUEST;

    // start socket
    sockfd = initSocketClient(SERVER_IP,SERVER_PORT);

    swrite(sockfd,&message,sizeof(message));

    /* wait server response */
    sread(sockfd, &message, sizeof(message));

    switch(message.code){   
        case INSCRIPTION_OK:
            printf("Réponse Serveur: INSCRIPTION acceptée.");
            break;
        case INSCRIPTION_KO:
            printf("Réponse Serveur: INSCRIPTION refusée.");
            break;

        default:
            printf("Réponse Serveur: non prevue %d.", message.code);
            break;
    }


    /*wait to know if the game START oR CANCEL*/
    sread(sockfd,&message,sizeof(message));

    if (message.code == START_GAME)
    {
        printf("START GAME\n");
        // pseudoPlayer code : 
        int* grille =  initGrille();
        
       while(sread(sockfd,&message,sizeof(message))){ 
            if (message.code==NUMERO_TUILE){
                tileNumber = message.tuile;
            
                do {
                    printf("Veuillez choisir un emplacement : \n");
                    printGrille(grille, GRILLE_SIZE);
                    sread(sockfd, &chosenPlacement, sizeof(chosenPlacement));
                } while (!choosePlacement(tileNumber, chosenPlacement, grille));
                
                // if true
                message.code = PLACEMENT_TERMINE;
                ret = swrite(sockfd,&message,sizeof(message));
                printf("Voici vos placement : \n");
                printGrille(grille,GRILLE_SIZE);
                cptGame++;
            }else{
                printf("Réponse Serveur: non prevue %d.", message.code);
            }
        }                
        
        // on a fini le jeux 
        if (cptGame==20){
            
            int scoreFinal = scoreCalculation(grille);
            Structplayer player ;
            strcpy(player.pseudo,pseudoPlayer);
            player.score=scoreFinal;
            ret = swrite(sockfd,&player,sizeof(player));


            while(sread(sockfd,&message,sizeof(message))){
                if ((message.code==RANKING)){
                    struct Structplayer* players;
                    int size;
                    sread(sockfd,&players,sizeof(players));
                    sread(sockfd,&size,sizeof(size));
                    printRanking(players,size);
                    free(players);
                }else{
                    printf("Réponse Serveur: non prevue %d.", message.code);                
                }
            }

            //  end 
            printf("END GAME");
            sclose(sockfd);

        }
            



    }else{
        printf("CANCEL GAME");
        sclose(sockfd);
    }

    return 0;
}

