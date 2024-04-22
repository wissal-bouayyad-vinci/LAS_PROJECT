#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "client.h"
#include "game.h"
#include "player.h"
#include "network_client.h"


volatile sig_atomic_t cptGame=0;


void printGrille(char** grille, int size){

    printf("GRILLE DE JEUX\n");

    for (int i = 0; i < size; ++i)
    {
        printf("case n°%d : %s  |\n  ", (i+1),grille[i]);
    }
}


void printRanking(struct Structplayer* players,int size){

    printf("RANKING FINAL\n");

    for (int i = 0; i < size; ++i)
    {
        printf("%d. %c with a score of %d\n",(i+1),players[i].pseudo, players[i].score);
    }

}


int main(int argc, char const *argv[])
{
    // attributs : 
    pseudo[MAX_PSEUDO];
    int sockfd;
    int ret;
    char tileNumber;
    int chosenPlacement;

    StructMessage message;

    // recup
    printf("Bienvenue a vous. Inscrivez vous pour commencer la partie.");
    printf("Pour cela votre pseudo : ");
    ret = sread(0,pseudo,MAX_PSEUDO);
    ckeckNeg(ret,"read client error");
    pseudo[ret - 1] = '\0';
    strcpy(message.messageText,pseudo);
    message.code = INSCRIPTION_REQUEST;

    // start socket
    sockfd = initSocketClient(SERVER_IP,SERVER_PORT);
    sockfdServer = getSocketServer(SERVER_PORT);

    swrite(sockfd,&message,sizeof(message));

    /* wait server response */
    sread(sockfdServer, &message, sizeof(msg));

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
    sread(sockfdServer,&message,sizeof(message));

    if (message.code == START_GAME)
    {
        printf("START GAME\n");
        // pseudo code : 
        char* grille =  initGrille();
        ;
       while(sread(sockfdServer,&message,sizeof(message))){ 
            if (message.code==NUMERO_TUILE){
                tileNumber = message.messageText;
            
                printf("Veuillez choisir un emplacement : \n");
                printGrille(grille,GRILLE_SIZE);
                sread(0,&chosenPlacement,1);
                char placeGrille = chosenPlacement+'0';
                bool PLACEMENT_MSG = choosePlacement(tileNumber,placeGrille,grille);
                while (!PLACEMENT_MSG){
                   bool PLACEMENT_MSG = choosePlacement(tileNumber,placeGrille,grille);                
                }
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
            StructPlayer player ;
            player.pseudo = pseudo;
            player.score=scoreFinal;
            ret = swrite(sockfd,&player,sizeof(player));


            while(sread(sockfdServer,&message,sizeof(message))){
                if (message.messageText==RANKING){
                    struct StructPlayer* players;
                    int size;
                    sread(sockfdServer,&players,sizeof(players));
                    sread(sockfdServer,&size,sizeof(size));
                    printRanking(players,size);
                
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

