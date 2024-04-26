#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdbool.h>
#include "config.h"
#include "message.h"
#include "utils_v1.h"
#include "ipc.h"
#include "network_serveur.h"
#include "server.h"
#include "player.h"
 
//GLOBALS VARIABLES
volatile sig_atomic_t end;
 
//*********************************************************************************//
//***********************************METHODES**************************************//
//*********************************************************************************//
 
/**
* PRE: /
* POST: on success creates a random displayed table containing all the necessary tiles.
* RES: returns a table of randomly displayed tiles, knowing that we have all of theese tiles :
* 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
* 11, 11, 12, 12, 13, 13, 14, 14,
* 15, 15, 16, 16, 17, 17, 18, 18,
* 19, 19, 20, 21, 22, 23, 24, 25,
* 26, 27, 28, 29, 30, 31;
*/
 
int* createTiles(){
    int* tilebag = (int*) malloc(NUMBER_OF_TILES*sizeof(int));
    if(tilebag == NULL) {
        perror("Allocation error");
        exit(1);
    }
 
    int index;
    int available [] = {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
        11, 11, 12, 12, 13, 13, 14, 14,
        15, 15, 16, 16, 17, 17, 18, 18,
        19, 19, 20, 21, 22, 23, 24, 25, 26,
        27, 28, 29, 30, 31
    };
 
    // Initialize tilebag with 0
    for (int i = 0; i < NUMBER_OF_TILES; i++) {
        tilebag[i] = 0;
    }
 
    for(int i=0 ; i<NUMBER_OF_TILES ; i++) {
        index = randomIntBetween(0,NUMBER_OF_TILES-1);
        while(tilebag[index]!= 0) {
            index++;
            if(index == NUMBER_OF_TILES){
                index=0;
            }
        }
        tilebag[index] = available[i];  
    }
 
    return tilebag;
}
 
/**
* PRE: tilebag: It is a Integer table with 40 randomly displayed tiles. 
*      nextTile: It is the index we use to dig the next tile in the tilebag. 
* POST: increments the nextTile, so the program knows which tile to dig next,
*       once it is called again.
* RES: returns the digged tile.
*/ 
int digTile(int* tilebag, int* nextTile) {
    int diggedTile = tilebag[*nextTile];
    (*nextTile)++;
    return diggedTile;
}
 
/**
* PRE: Player: The new player we are going to put in the table of players 
*      nbPlayers: number of Players already in the table
* POST: The playersTable will contain a new player.
*       The number of players will be incremented, due to the new add.
* RES: /
*/
void addPlayerToTable (Structplayer* tableauDesJoueurs,Structplayer newPlayer, int* nbPlayers){
    //PUT THE NEW PLAYER'S PSEUDO AND SCORE IN THE TABLE
    strncpy(tableauDesJoueurs[*nbPlayers].pseudo, newPlayer.pseudo,strlen(newPlayer.pseudo));
    tableauDesJoueurs[*nbPlayers].pseudo[MAX_PSEUDO -1] = '\0';
    tableauDesJoueurs[*nbPlayers].score = 0;
    (*nbPlayers)++;
}
 
//*********************************************************************************//
//******************************CHILD TREATMENT************************************//
//*********************************************************************************//
 
//CHILD_TREATMENT
void child_trt(void *pipefdIn, void *pipefdOut) {
    int *pipefdO = pipefdOut;
    int *pipefdI = pipefdIn;
 
    //CLOTURE DU DESCRIPTEUR D'ECRITURE SUR LE PIPE
    sclose(pipefdI[1]);
    sclose(pipefdO[0]);
 
    //ON ATTEND UN MESSAGE DE LA PART DU PERE 
    StructMessage msg;
    sread(pipefdI[0],&msg,sizeof(StructMessage));
    printf("Message reçu du père : %s",msg.messageText);
 
    //ENVOYER CE MESSAGE AU CLIENT:
    char* message = "Message bien recu";
    // swrite(newsockfd, &msg, sizeof(StructMessage));
    swrite(pipefdO[1], &message, sizeof(message));
 
    //FERMER LA CONNECTION AVEC LE CLIENT
    // sclose(newsockfd);
}
 
//*********************************************************************************//
//*****************************END_SERVER_HANDLER**********************************//
//*********************************************************************************//
 
 
void endServerHandler (int sig) {
    end = 1;
}
 
//*********************************************************************************//
//****************************************MAIN*************************************//
//*********************************************************************************//
 
int main(int argc, char const *argv[]) {
 
    //INITIALISER LE SOCKET SERVER 
    int sockfd = initSocketServer(SERVER_PORT);
    printf("Le serveur tourne\n");
 
    //VARIABLES
    end = 0;
    int nbPlayers = 0;
    // int indexPiocherTuile = 0;
    int newsockfd;
    StructMessage msg;
    Structplayer* tabPlayers = malloc (MAX_PLAYERS * sizeof(Structplayer));
    if(!tabPlayers){
        perror("ALLOCATION ERROR");
        exit(1);
    }
 
    //ALARMES
    ssigaction (SIGALRM, endServerHandler);
    //CREATION DU PIPE POUR COMMUNICATION AVEC FILS
    //Après le fork on aura deux pipes un pour le fils et un pour le pere
    int pipefdIn[2];
    int pipefdOut[2];
    spipe(pipefdIn);
    spipe(pipefdOut);
 
    
 
    
 
    //*********************************************************************************//
    //*******************************INSCRIPTIONS**************************************//
    //*********************************************************************************//
 
    alarm(TIME_INSCRIPTION);
 
    while (!end) {
        //CLIENT TREATMENT
        // saccept() exit le programme si accept a été interrompu par l'alarme
        newsockfd = accept(sockfd,NULL,NULL);
        //newsockfd > 0 si il n'y a pas eu d'erreur avec acceppt
        if(newsockfd >0) {
            sread(newsockfd, &msg, sizeof(msg));
 
            if(msg.code == INSCRIPTION_REQUEST) {
                printf("Inscription demandée par le joueur : %s\n",msg.messageText);
 
                if (nbPlayers < MAX_PLAYERS) {
                    msg.code = INSCRIPTION_OK;
 
                    Structplayer newPlayer;
                    strcpy(newPlayer.pseudo, msg.messageText);
                    newPlayer.score = 0;
                    newPlayer.sockfd = newsockfd;
 
                    addPlayerToTable(tabPlayers , newPlayer, &nbPlayers);
                    //CREATION DE L'ENFANT
                    fork_and_run2(child_trt,pipefdIn,pipefdOut);
 
                    //Soit on a trouvé 3 personnes en 30sec soit on arrete de rechercher des joueurs après 30 et on doit vérifier qu'on a au moins 2 joueurs.
                    if(nbPlayers == MAX_PLAYERS) {
                        alarm(0); //On a atteint le max de joueurs pour une partie 
                    }
                } else {
                    msg.code = INSCRIPTION_KO;
                }
                swrite(newsockfd, &msg , sizeof(msg));
                printf("Nombre d'inscriptions : %i\n", nbPlayers);
            }
        }
    }
 
    printf("FIN DES INSCRIPTIONS\n");
 
    //*********************************************************************************//
    //*******************************ANNULER PARTIE************************************//
    //*********************************************************************************//
 
    //CLOTURE DU DESCRIPTEUR POUR LA LECTURE SUR LE PIPE D'ECRITURE
    sclose(pipefdOut[0]);
    //CLOTURE DU DESCRIPTEUR POUR L'ECRITURE SUR LE PIPE DE LECTURE
    sclose(pipefdIn[1]);
 
    if(nbPlayers < MIN_PLAYERS) {
        printf("PARTIE ANNULEE ... PAS AU MOINS 2 JOUEURS");
        msg.code  = CANCEL_GAME;
        char* message = "Partie annulée : joueurs insuffisants";
        strncpy(msg.messageText,message,strlen(message));
        //ON ECRIT UN MESSAGE POUR LE SERVEUR FILS
        swrite(pipefdOut[1], &msg, sizeof(StructMessage));
 
        //On recoit un message du serveur fils 
        sread(pipefdIn[0], &msg, strlen(message));
        printf("Message de mon fils %s:",message);
        // ON CLOTURE LES DEUX PIPES
        sclose(pipefdOut[1]);
        sclose(pipefdIn[0]);
 
    } else{
 
    //*********************************************************************************//
    //*******************************COMMENCER PARTIE************************************//
    //*********************************************************************************//
    int* tilesbag = createTiles();
    free(tilesbag);
    free(tabPlayers);
}
    exit(0);
}
/*
    //SHARED MEMORY & SEMAPHORE
    int shmid = sshmget(SHM_KEY, nbrJoueurs * sizeof(int), 0);
    int sid = sem_get(SEM_KEY, 1);
 
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
 
 
void setScorePlayerIPC(player.pseudo,player.score){
    recuperer joueur dans la memoire partagee et changer direct le score.
}
 
void trierRankingParScore(){
    creer un nouveau tableau struct Joueurs organisé par score décroissant.
}
 
*/
 
