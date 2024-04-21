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

void printGrille(char* grille, int size){

    printf("GRILLE DE JEUX\n");

    for (int i = 0; i < size; ++i)
    {
        printf("case n°%d : %c  |\n  ", (i+1),grille[i]);
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

    swrite(sockfd,&message,sizeof(message));

    /* wait server response */
    sread(sockfd, &message, sizeof(msg));

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
        // pseudo code : 
        char* grille =  initGrille();
        sread(numeroTuile);
        while(receive a numero tuile){
            printf("Veuillez choisir un emplacement");
            printGrille(grille,GRILLE_SIZE);
            sread(le numero choisi par le joueur);
            bool PLACEMENT_MSG = choosePlacement(int tileNumber, int chosenPlace,grille);
            // if false : on redemande ?
            // if true
            ret = swrite(envoyer le PLACEMENT_MSG au server);
            printf("Voici placer : ");
            printGrille(grille,GRILLE_SIZE);

        }
        
        // on a fini le jeux 

        int scoreFinal = scoreCalculation(grille);
        StructPlayer player ;
        player.pseudo = pseudo;
        player.score=scoreFinal;
        ret = swrite( player);


        sread();
        sread();
        printRanking(playersTable,size);

        //  end 

    }else{
        printf("CANCEL GAME");
        sclose(sockfd);
    }

    return 0;
}















