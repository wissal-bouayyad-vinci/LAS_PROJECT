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

//CHILD_TREATMENT
static void child_trt(void *pipeV)
{
    int *pipefd = pipeV;

    //CLOTURE DU DESCRIPTEUR D'ECRITURE SUR LE PIPE
    int ret = sclose(pipefd[1]);

    //ON ATTEND UN MESSAGE DE LA PART DU PERE 
    StructMessage msg;
    int nbChar = sread(pipefd[0],&msg,sizeof(StructMessage));
    printf("Message reçu du père : %s",msg.messageText);

    //ENVOYER CE MESSAGE AU CLIENT:
    swrite(newsockfd, &msg, sizeof(StructMessage));

    //FERMER LA CONNECTION AVEC LE CLIENT
    sclose(newsockfd);
    //FERMER LA CONNECTION D'ÉCOUTE
    sclose(sockfd);
    //ON CLÔTURE LE CÔTE LECTURE DU PIPE
    ret = sclose(pipefd[0]);

}

void endServerHandler(int sig)
{
    end_inscriptions = 1;
}

int main(int argc, char const *argv[])
{

    //Initialiser memoire avec methode;
    int indexPiocherTuile = 0;
    int sockfd, newsockfd;
    StructMessage msg;
    int ret;
    struct pollfd fds[MAX_PLAYERS];

    //ALARMES
    ssigaction (SIGALRM, endServerHandler);

    //INITIALISER LE SOCKET SERVER 
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

                    //CREATION DU PIPE POUR COMMUNICATION AVEC FILS
                    //Après le fork on aura deux pipes un pour le fils et un pour le pere
                    int pipefd[2];
                    int ret2 = spipe(pipefd);

                    //CREATION DE L'ENFANT
                    fork_and_run1(child_trt,pipefd);

                    //CLOTURE DU DESCRIPTEUR POUR LA LECTURE SUR LE PIPE
                    ret = sclose(pipefd[0]);

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
        msg.code  = CANCEL_GAME;
        msg.messageText = "Partie annulée : joueurs insuffisants";
        
        //ON ECRIT UN MESSAGE POUR LE SERVEUR FILS
        int nbChar = swrite(pipefd[1], &msgcode, sizeof(StructMessage));

        // ON CLOTURE LE CÔTÉ ÉCRITURE DU PIPE
        ret2 = sclose(pipefd[1]);

    } else {

    //*********************************************************************************//
    //*******************************COMMENCER PARTIE************************************//
    //*********************************************************************************//

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
    int tilebag [NUMBER_OF_TILES];
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
    int diggedTile = tilebag[nextTile];
    nextTile++;
    return diggedTile;
}

/**
 * PRE: fds: structure that contains every fd of every signed in player
 *      nbPlayers: number of Players in the game
 * POST: /
 * RES: returns a table of players
 */
void Structplayer* createPlayersTable (struct pollfd fds, int nbPlayers){
    //CREATE TABLE OF PLAYERS STRUCT
    Structplayer* tableauDesJoueurs = malloc (nbPlayers * sizeof(Structplayer));
    if(!tableauDesJoueurs){
        perror("Error in memory allocation");
        exit(1);
    }
    
    //PUT THE PLAYERS PSEUDOS AND SCORES IN THE TABLE
    for(int i=0; i<nbPlayers; i++){
        strncpy(tableauDesJoueurs[i].pseudo, fds[i].pseudo, MAX_PSEUDO -1);
        tableauDesJoueurs[i].pseudo[MAX_PSEUDO -1] = '\0';
        tableauDesJoueurs[i].score = 0;
    }

    return tableauDesJoueurs; 
}
}


