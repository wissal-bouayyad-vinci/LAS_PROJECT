#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "message.h"
#include "utils_v1.h"
#include "config.h"
#include "ipc.h"
#include "network_serveur.h"
#include "server.h"
#include "player.h"

//GLOBALS VARIABLES
Player tabPlayers[MAX_PLAYERS];
volatile sig_atomic_t end_inscriptions = 0;

void endServerHandler(int sig)
{
    end_inscriptions = 1;
}

int main(int argc, char const *argv[])
{
    //Initialiser memoire avec methode;
    int indexPiocherTuile = 0;
    int sockfd;
    StructMessage msg;
    int ret;
    struct pollfd fds[MAX_PLAYERS];
    ssigaction (SIGALRM, endServerHandler);

    //Initialize server 
    sockfd = initSocketServer(SERVER_PORT);
    printf("Le serveur tourne sur le port : %i \n",SERVER_PORT);

    int i = 0;
    int nbPlayers = 0;

    //*********************************************************************************//
    //*******************************INSCRIPTIONS**************************************//
    //*********************************************************************************//

    alarm(TIME_INSCRIPTION);

    while (!end_inscriptions) {
        //CLIENT TREATMENT
        // saccept() exit le programme si accept a été interrompu par l'alarme
        newsockfd = accept(sockfd,NULL,NULL);
        //newsockfd > 0 si il n'y a pas eu d'erreur avec acceppt
        if(newsockfd >0) {
            ret = sread(newsockfd, &msg, sizeof(msg));

            if(msg.code == INSCRIPTION_REQUEST) {
                printf("Inscription demandée par le joueur : %s\n",msg.messageText);
                strcpy(tabPlayers[i].pseudo, msg.messageText);
                tabPlayers[i].sockfd = newsockfd;
                i++;

                if (nbPlayers < MAX_PLAYERS) {
                    msg.code = INSCRIPTION_OK;
                    nbPlayers++;

                    //Soit on a trouvé 3 personnes en 30sec soit on arrete de rechercher des joueurs après 30 et on doit vérifier qu'on a au moins 2 joueurs.
                    if(nbPlayers == MAX_PLAYERS) {
                        alarm(0); //On a atteint le max de joueurs pour une partie 
                    }
                } else {
                    msg.code = INSCRIPTION_KO;
                }
                ret = swrite(newsockfd, &msg , sizeof(msg));
                printf("Nombre d'inscriptions : %i\n", nbPlayers);
            }
        }
    }

    printf("FIN DES INSCRIPTIONS\n");

    //*********************************************************************************//
    //*******************************ANNULER PARTIE************************************//
    //*********************************************************************************//

    if(nbPlayers < MIN_PLAYERS) {
        printf("PARTIE ANNULEE ... PAS AU MOINS 2 JOUEURS");
        msgcode  = CANCEL_GAME;
            ret = swrite(fds[0].sockfd, &msg,sizeof(msg));

    }

    //*********************************************************************************//
    //*******************************COMMENCER PARTIE************************************//
    //*********************************************************************************//

    msgcode =START_GAME;

    //Mettre dans l'IPC un tableau avec des structPlayers


    for(fds){
        ret = swrite(fds[i], &msg, sizeof(msg));
    }

    //appeller methode crer tuiles
    tableau tuiles = createTuiles;

    //Commencer les 20 tours

    for(20){
    //piocher tuile
    char numeroTuile = piocher une tuile(tuiles);

    //envoyerTuile A tous les joueurs
    for(int i=0; nbrJoueurs){
        ret = swrite(fds[i],&numeroTuile, sizeof(char));
    }
    while(pasRecu nbrJoueurs PLACEMENT_TUILES){
        sleep(1);
    }
    }

    //Je sors du for et je mets en place le sémaphore
    // Recevoir 3 fois MES_POIǸTS

    -> setplayerIPC(player.pseudo,player.score);

    semaphore up.

/**
 * POST: on success creates a random displayed table containing all the necessary tiles.
 * RES: returns a char table of randomly displayed tiles of tuiles, knowing that we have all of theese tiles :
 * 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, J;
 */

char* createTiles(){
    char* tilebag [TOTAL_TILES]; //TOTAL_TILES = 40;
    int index;
    for(int i=0 ; i<30 ; i++) {
        index = randomIntBetween(0,TOTAL_TILES-1);
        while(tilebag[index]!=NULL) {
            i++;
            if(index == TOTAL_TILES){
                index=0;
            }
        }
        tilebag[index] = i + '\0';
    }
    for(int i=11 ; i<20 ; i++) {
        index = randomIntBetween(0,TOTAL_TILES-1);
        while(tilebag[index]!=NULL) {
            i++;
            if(index == TOTAL_TILES){
                index=0;
            }
        }
        tilebag[index] = i + '\0';
    }
    for(int i=0 ; i<TOTAL_TILES;i++){
        if(tilebag[i]!= NULL){
            tilebag[i]='J';
            break;
        }
    }    
}



char piocherTuile(char* tableau, int* index) {
    char numeroTuile = tableau[index];
    index++;
    return numeroTuile;
}

// char envoyerTuile(fds, nbrJoueurs, numeroTuile);
void creerTableauAvecStructPlayers(fds){
    mettre dans IPC tous les joueurs;
}

void setScorePlayerIPC(player.pseudo,player.score){
    recuperer joueur dans la memoire partagee et changer direct le score.
}

void trierRankingParScore(){
    creer un nouveau tableau struct Joueurs organisé par score décroissant.
}


    //*********************************************************************************//
    //***********************************METHODES**************************************//
    //*********************************************************************************//


}