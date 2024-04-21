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
    int sockfd;
    StructMessage msg;
    struct pollfd fds[MAX_PLAYERS];
    ssigaction (SIGALRM, endServerHandler);

    //Initialize server 
    sockfd = initSocketServer(SERVER_PORT);
    printf("Le serveur tourne sur le port : %i \n",SERVER_PORT);

    i = 0;
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
        printf("PARTIE ANNULEE ... PAS AU MOINS 2 JOUEURS")
    }
    return 0;
}