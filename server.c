#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdbool.h>
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
 
int digTile(int* tilebag, int* nextTile) {
    int diggedTile = tilebag[*nextTile];
    (*nextTile)++;
    return diggedTile;
}

void addPlayerToTable (Structplayer* tableauDesJoueurs,Structplayer newPlayer, int* nbPlayers){
    //PUT THE NEW PLAYER'S PSEUDO AND SCORE IN THE TABLE
    strncpy(tableauDesJoueurs[*nbPlayers].pseudo, newPlayer.pseudo,strlen(newPlayer.pseudo));
    tableauDesJoueurs[*nbPlayers].pseudo[MAX_PSEUDO -1] = '\0';
    tableauDesJoueurs[*nbPlayers].score = 0;
    tableauDesJoueurs[*nbPlayers].sockfd = newPlayer.sockfd;  
    (*nbPlayers)++;
}

void sortByScoreDescending(Structplayer *players, int nbPlayers){
    int i, j;
    Structplayer temp;
    
    for (i = 1; i < nbPlayers; i++) {
        temp = players[i];
        j = i - 1;
        
        // Move elements of players[0..i-1], that are greater than temp, to one position ahead of their current position
        while (j >= 0 && players[j].score < temp.score) {
            players[j + 1] = players[j];
            j = j - 1;
        }
        players[j + 1] = temp;
    }
} 
 
//*********************************************************************************//
//******************************CHILD TREATMENT************************************//
//*********************************************************************************//
 
//CHILD_TREATMENT
void child_trt(void *pipefdOut, void *pipefdIn, void *socket) {
    int *pipefdO = pipefdOut;
    int *pipefdI = pipefdIn;
    int *newsocket = socket;
 
    //CLOTURE DU DESCRIPTEUR D'ECRITURE SUR LE PIPE
    sclose(pipefdI[1]);
    sclose(pipefdO[0]);
 
    //ON ATTEND UN MESSAGE DE LA PART DU PERE : PARTIE ANNULEE OU START GAME 
    StructMessage msg;
    sread(pipefdI[0],&msg,sizeof(msg));
    printf("Code du message recu du pere : %d\n ",msg.code);
 
    //ENVOYER LE MESSAGE DU PERE AU CLIENT
    swrite(*newsocket, &msg, sizeof(msg));
    printf("Code du message recu du pere : %d\n ",msg.code);
    if(msg.code == START_GAME){ 
        //RECEVOIR NUMERO TUILE PERE 20 FOIS
        for(int i=0 ; i< NUMBER_OF_USED_TILES; i++){
            sread(pipefdI[0],&msg,sizeof(msg));
            printf("numero tuile : %d \n",msg.tuile);
            swrite(*newsocket,&msg,sizeof(msg));
            sread(*newsocket,&msg,sizeof(msg));
            if(msg.code!=PLACEMENT_TERMINE){
                printf("message non attendu : %d \n",msg.code);
            }    
            swrite(pipefdO[1], &msg,sizeof(msg));
            printf("%d placement terminer envoyer\n ", i);
        } 
    }


    //RECEVOIR LE SCORE DU CLIENT
    int score;
    sread(*newsocket, &score, sizeof(int));
    printf("Mon score chez le fils vaut : %d",score);
    //ENVOYER LE SCORE AU PERE
    swrite(pipefdO[1],&score, sizeof(int));

    //ENVOYER MESSAGE RANKING
    msg.code = RANKING;
    char* message = "Voici le ranking de tous les joueur\n\0";
    strcpy(msg.messageText,message);
    swrite(*newsocket,&msg,sizeof(msg));
    //Envoyer le nombre de joueurs
    int nbrJoueurs;
    sread(pipefdI[0],&nbrJoueurs,sizeof(int));
    swrite(*newsocket,&nbrJoueurs,sizeof(int));

    //ACCEDER A LA MEMOIRE PARTAGEE PROTEGEE PAR SEMAPHORE
    Structplayer* playersRanking = getsharedMemory();
    swrite(*newsocket, &playersRanking, sizeof(Structplayer));
    
    //FERMER LA CONNECTION AVEC LE CLIENT
    sclose(*newsocket);
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
    int sockfd = initSocketServer();
    printf("Le serveur tourne\n");
    
    //VARIABLES
    end = 0;
    int nbPlayers = 0;
    int newsockfd;
    int nextTile = 0;
    StructMessage msg;
    Structplayer* tabPlayers = malloc (MAX_PLAYERS * sizeof(Structplayer));
    if(!tabPlayers){
        perror("ALLOCATION ERROR");
        exit(1);
    }
 
    //ALARMES
    ssigaction (SIGALRM, endServerHandler);

 
    //*********************************************************************************//
    //*******************************INSCRIPTIONS**************************************//
    //*********************************************************************************//
 
    alarm(15);
 
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
                    //Soit on a trouvé 3 personnes en 30sec soit on arrete de rechercher des joueurs après 30 et on doit vérifier qu'on a au moins 2 joueurs.
                    if(nbPlayers == MAX_PLAYERS) {
                        alarm(0); //On a atteint le max de joueurs pour une partie 
                        end = 1;
                    }
                } else {
                    msg.code = INSCRIPTION_KO;
                }
                swrite(newsockfd, &msg , sizeof(msg));
                printf("Nombre d'inscriptions : %i\n", nbPlayers);
            }
        }
    }
    
    //CREATION DE MEMOIRE PARTAGEE ET CREATION DE SEMAPHORE
    int shmId = initSharedMemory(nbPlayers);
    Structplayer* tableJoueursIPC = attacheSHM(shmId);
    int semID = initSemaphore();

    //CREATION DES PIPES POUR LA COMMUNCATION ENTRE PERE ET FILS
    typedef struct {
        int pipefdRead[2];
        int pipefdWrite[2]; 
    }structPipe;

    structPipe* pipes = malloc (nbPlayers*sizeof(structPipe));
    if(!pipes){
        perror("Error in allocation");
        exit(1);
    }   
    printf("FIN DES INSCRIPTIONS\n");

    for(int i=0 ; i<nbPlayers; i++){
        int pipefdI[2];
        int pipefdO[2];  
        spipe(pipefdI);
        spipe(pipefdO);

        pipes[i].pipefdRead[0]  = pipefdI[0] ;
        pipes[i].pipefdRead[1]  = pipefdI[1] ;
        pipes[i].pipefdWrite[0]  = pipefdO[0] ;
        pipes[i].pipefdWrite[1]  = pipefdO[1] ; 
        fork_and_run3(child_trt,pipes[i].pipefdRead, pipes[i].pipefdWrite , &tabPlayers[i].sockfd);
    
        //CLOTURE DU DESCRIPTEUR POUR LA LECTURE SUR LE PIPE D'ECRITURE
        sclose(pipes[i].pipefdWrite[0]);
        //CLOTURE DU DESCRIPTEUR POUR L'ECRITURE SUR LE PIPE DE LECTURE
        sclose(pipes[i].pipefdRead[1]);    
    }

    //*********************************************************************************//
    //*******************************ANNULER PARTIE************************************//
    //*********************************************************************************//
 
    if(nbPlayers < MIN_PLAYERS) {
        printf("PARTIE ANNULEE ... PAS AU MOINS 2 JOUEUR\n");
        msg.code  = CANCEL_GAME;
        char* message = "Partie annulée\n";
        strcpy(msg.messageText, message);
        //ON ECRIT UN MESSAGE POUR TOUS LES SERVEURS FILS
        for(int i=0 ; i<nbPlayers ; i++){
            swrite(pipes[0].pipefdWrite[1] , &msg, sizeof(StructMessage));
        } 
    
    } else {
 
    //*********************************************************************************//
    //*******************************COMMENCER PARTIE************************************//
    //*********************************************************************************//
    printf ("La jeu va commencer\n");
    msg.code = START_GAME;
    printf ("Message que le père encode : %s; Code du message : %d\n",msg.messageText,msg.code);

    for(int i= 0 ; i<nbPlayers; i++){
        swrite(pipes[i].pipefdWrite[1],&msg,sizeof(StructMessage));
    } 

    int* tilesbag = createTiles();
    int cptPlacedTiles = 0;
    int tileNumber;
    int cptPlacement =0;
    for (int i=0 ; i<NUMBER_OF_PLAYS;i++){
        //ENVOYER TUILE AU FILS
        tileNumber = digTile(tilesbag,&nextTile);
        msg.code = NUMERO_TUILE;
        msg.tuile = tileNumber;

        for(int i=0 ; i<nbPlayers ; i++){
            swrite(pipes[i].pipefdWrite[1] , &msg, sizeof(StructMessage));
        } 


        for(int i=0 ; i<nbPlayers ; i++){
            sread(pipes[i].pipefdRead[0] , &msg, sizeof(msg));
            printf("fils envoie %d placement terminer \n ",cptPlacement);
            if(msg.code == PLACEMENT_TERMINE){
                cptPlacedTiles++;
                cptPlacement++;
            } 
        }


    }
    printf("Les 20 tours sont terminés!\n ");
    msg.code= END_TOUR;
    for (int i = 0; i < nbPlayers; ++i){
        swrite(pipes[i].pipefdWrite[1],&msg,sizeof(msg));
    }

    //ON ENTRE DANS LA ZONE CRITIQUE
    int scorePersonnel;
    sem_down0(semID);
    // JE RECUPERE TOUS LES SCORES DES JOUEURS ET JE LE METS DANS LA TABLE
    for(int i=0 ;i<nbPlayers;i++){
        sread(pipes[i].pipefdRead[0],&scorePersonnel,sizeof(int));
        tabPlayers[i].score = scorePersonnel; 
    }

    //TRIER LES SCORES PAR ORDRE DECROISSANTE ET METTRE DANS MEMOIRE PARTAGEE
    sortByScoreDescending(tabPlayers,nbPlayers);

    //Mettre la table trié dans l'ipc
    for(int i=0 ; i<nbPlayers; i++){
        tableJoueursIPC[i] = tabPlayers[i];
    } 
    //ON SORT DE LA ZONE CRITIQUE
    sem_up0 (semID); 
    
    free(tilesbag);
    free(tabPlayers);

    }
    // ON CLOTURE TOUTES LES PIPES
    for(int i=0 ; i<nbPlayers ; i++){
        sclose(pipes[i].pipefdWrite[1]);
        sclose(pipes[i].pipefdRead[0]);
    } 
    sshmdelete(shmId);
    //attendre fils ou pas? 
    exit(0);
} 

 
