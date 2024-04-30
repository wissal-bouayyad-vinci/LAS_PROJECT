#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <fcntl.h>
#include <ctype.h> 
#include "message.h"
#include "utils_v1.h"
#include "ipc.h"
#include "network_serveur.h"
#include "server.h"
#include "player.h"
 
//GLOBALS VARIABLES
volatile sig_atomic_t end;
volatile sig_atomic_t endGame;


//*********************************************************************************//
//***********************************METHODES**************************************//
//*********************************************************************************//

 void freeAllMemory(int shmId, int sem_id,Structplayer* tableJoueursIPC){
    //SUPPRESSION MEMOIRE PARTAGEE
    sshmdt(tableJoueursIPC);
    sshmdelete(shmId);
    sem_delete(sem_id);
 }


 void freeAllPipes(structPipe* pipes, int nbPlayers){
    for(int i=0 ; i<nbPlayers ; i++){
        //FERMETURE DE TOUS LES PIPES CÔTE PÈRE
        sclose(pipes[i].pipefdWrite[1]);
        sclose(pipes[i].pipefdRead[0]);
    } 

 }

 void freeAllSocket(Structplayer* tabPlayers, int nbPlayers){
    for(int i=0 ; i<nbPlayers ; i++){
        //FERMETURE DE TOUS LES SOCKETS CLIENTS
        sclose(tabPlayers[i].sockfd);
    } 
 }


void freeAllMalloc(Structplayer* tabPlayers, int* fils)  {
    free(tabPlayers) ;
    free(fils) ;
} 


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
 
    // ATTRIBUTS FILS
    Structplayer* playersRankingSHM ;
    Structplayer* playersRanking ;


    //ON ATTEND UN MESSAGE DE LA PART DU PERE : PARTIE ANNULEE OU START GAME 
    StructMessage msg;
    sread(pipefdI[0],&msg,sizeof(msg));
    if (msg.code==CANCEL_GAME){
        printf("Code du message recu du pere : %d\n ",msg.code);
        swrite(*newsocket, &msg, sizeof(msg));
    }
    else if (msg.code==START_GAME)
    {
        
        swrite(*newsocket, &msg, sizeof(msg));
        
        //RECEVOIR NUMERO TUILE PERE 20 FOIS
        for(int i=0 ; i< NUMBER_OF_USED_TILES; i++){

            sread(pipefdI[0],&msg,sizeof(msg));
            
            if (msg.code==NUMERO_TUILE){
                printf("numero tuile : %d \n",msg.tuile);
                swrite(*newsocket,&msg,sizeof(msg));                
            }

            sread(*newsocket,&msg,sizeof(msg));
            if (msg.code==PLACEMENT_TERMINE){
                swrite(pipefdO[1], &msg,sizeof(msg));
                printf("%d placement terminer envoyer\n ", i);
            }
            
        } 
        
    }

    // RECEVOIR LE MESSAGE MON_SCORE
    sread(*newsocket, &msg, sizeof(msg));
    if (msg.code==MON_SCORE){
        printf("fils : recevoir score client\n");
        //RECEVOIR LE SCORE DU CLIENT
        int score;
        sread(*newsocket, &score, sizeof(int));
        printf("Mon score chez le fils vaut : %d\n",score);

        //ENVOYER LE SCORE AU PERE
        swrite(pipefdO[1],&score, sizeof(int));

    }
    
    //ENVOYER MESSAGE RANKING
    sread(pipefdI[0],&msg,sizeof(msg));
    if (msg.code==RANKING){
        swrite(*newsocket,&msg,sizeof(msg));
        printf("envoie message ranking : %d\n ", msg.code);
        //Envoyer le nombre de joueurs
        int nbrJoueurs;
        sread(pipefdI[0],&nbrJoueurs,sizeof(int));
        swrite(*newsocket,&nbrJoueurs,sizeof(int));

        //ACCEDER A LA MEMOIRE PARTAGEE PROTEGEE PAR SEMAPHORE
        int semID = getSemaphore();
        sem_down0(semID);
        playersRankingSHM = getsharedMemory(nbrJoueurs);
        playersRanking = malloc (nbrJoueurs * sizeof(Structplayer));

        if(!playersRanking){
            perror("ALLOCATION ERROR");
            exit(1);
        }
        
        for(int i=0 ; i<nbrJoueurs ; i++){
            strcpy(playersRanking[i].pseudo, playersRankingSHM[i].pseudo);
            playersRanking[i].score = playersRankingSHM[i].score;
            swrite(*newsocket,&playersRanking[i],sizeof(Structplayer)); 
        } 

        sem_up0(semID);
    }
    
    

    //ENVOYER ENDGAME AU CLIENT
    sread(pipefdI[0] ,&msg,sizeof(msg));
    if (msg.code == END_GAME){
        swrite(*newsocket,&msg,sizeof(msg));
    }

    //FERMER LA COMMUNICATION AVEC LE PERE
    free(playersRanking);
    sclose(pipefdI[0]);
    sclose(pipefdO[1]);
}
 
//*********************************************************************************//
//*****************************END_SERVER_HANDLER**********************************//
//*********************************************************************************//
 
 
void endServerHandler (int sig) {
    end = 1;
}


//*********************************************************************************//
//*****************************END_GAME_HANDLER**********************************//
//*********************************************************************************//
 
 
void endGameHandler (int sig) {
    endGame = 1;
}
 
//*********************************************************************************//
//****************************************MAIN*************************************//
//*********************************************************************************//
 
int main(int argc, char const *argv[]) {

    // SET ACTION
    sigset_t blocked;
    ssigemptyset(&blocked);
    ssigaddset(&blocked, SIGINT);
    ssigprocmask(SIG_BLOCK, &blocked, NULL);

    //ALARMES
    ssigaction (SIGALRM, endServerHandler);
    ssigaction(SIGINT,endGameHandler);

    
 
    //INITIALISER LE SOCKET SERVER
    if(argc < 2){
        perror("Missing port");
        exit(1);
    }
    int port = atoi(argv[1]); 
    int sockfd = initSocketServer(port);
    printf("Le serveur tourne dans le port %d\n",port);



    //Recuperer le fichier tuiles
    int* tilesTemp;
    int cpt=0;
    FILE *fdTuiles;
    if(argc == 3){
        fdTuiles = fopen(argv[2],"r");
        int nbr;
        while(fscanf(fdTuiles, "%d", &nbr)==1){
            cpt++;
        }

        tilesTemp =malloc(cpt*sizeof(int));
        if (tilesTemp==NULL){
            perror("tilesTemp error allocation");
            exit(1);
        }

        cpt=0;
        while(fscanf(fdTuiles, "%d", &tilesTemp[cpt])==1){
            cpt++;
        }
    }

    int nextTileTemp=0; 
    int* tilesbag;

    

    // SI ON RRETE LE JEUX APRES UNE PARTIE (CONTROL-C) 
    while(!endGame){

        ssigprocmask(SIG_BLOCK, &blocked, NULL);

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
 
    
        //*********************************************************************************//
        //*******************************INSCRIPTIONS**************************************//
        //*********************************************************************************//
     
        printf("LES INSCRIPTIONS VONT COMMENCER\n");
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
        

        ssigprocmask(SIG_UNBLOCK, &blocked, NULL);

        if (!endGame){
            ssigprocmask(SIG_BLOCK, &blocked, NULL);

            // ATTRIBUTS DU PERE
            int shmId ;
            Structplayer* tableJoueursIPC;
            int semID; 
            int* fils;           
            structPipe* pipes;

            // CREATION PIPES POUR COMMUNICATION AVEC FILS
            if (nbPlayers>0){
                        
                pipes = malloc (nbPlayers*sizeof(structPipe));
                if(!pipes){
                    perror("Error in allocation");
                    exit(1);
                }

                printf("FIN DES INSCRIPTIONS\n");

                fils= malloc (nbPlayers*sizeof(int));
                if(!fils){
                    perror("Error in allocation");
                    exit(1);
                }   

                for(int i=0 ; i<nbPlayers; i++){
                    int pipefdI[2];
                    int pipefdO[2];  
                    spipe(pipefdI);
                    spipe(pipefdO);

                    pipes[i].pipefdRead[0]  = pipefdI[0] ;
                    pipes[i].pipefdRead[1]  = pipefdI[1] ;
                    pipes[i].pipefdWrite[0]  = pipefdO[0] ;
                    pipes[i].pipefdWrite[1]  = pipefdO[1] ; 
                    int idFils = fork_and_run3(child_trt,pipes[i].pipefdRead, pipes[i].pipefdWrite , &tabPlayers[i].sockfd);
                    fils[i] = idFils;
                    //CLOTURE DU DESCRIPTEUR POUR LA LECTURE SUR LE PIPE D'ECRITURE
                    sclose(pipes[i].pipefdWrite[0]);
                    //CLOTURE DU DESCRIPTEUR POUR L'ECRITURE SUR LE PIPE DE LECTURE
                    sclose(pipes[i].pipefdRead[1]);    
                }
    }

        //*********************************************************************************//
        //*******************************ANNULER PARTIE************************************//
        //*********************************************************************************//
     
        if(nbPlayers < MIN_PLAYERS && nbPlayers>0) {
            printf("PARTIE ANNULEE ... PAS AU MOINS 2 JOUEUR\n");
            msg.code  = CANCEL_GAME;
            char* message = "Partie annulée\n";
            strcpy(msg.messageText, message);
            //ON ECRIT UN MESSAGE POUR TOUS LES SERVEURS FILS
            for(int i=0 ; i<nbPlayers ; i++){
                swrite(pipes[i].pipefdWrite[1] , &msg, sizeof(StructMessage));
            } 

            freeAllPipes(pipes,nbPlayers);
        
        } 

        //*********************************************************************************//
        //*******************************AUCUNE INSCRIPTION FAITE************************************//
        //*********************************************************************************//
     
        else if(nbPlayers==0){
            printf("Aucun joueur ne s'est inscrit.\n");
        }

        //*********************************************************************************//
        //*******************************COMMENCER PARTIE************************************//
        //*********************************************************************************//
            
        else if (nbPlayers>=MIN_PLAYERS)
        {

            //CREATION DE MEMOIRE PARTAGEE ET CREATION DE SEMAPHORE
            shmId = initSharedMemory(nbPlayers);
            tableJoueursIPC = attacheSHM(shmId);
            semID = initSemaphore();
     
            // START GAME
            printf ("La jeu va commencer\n");
            msg.code = START_GAME;
            char* message = "Partie va commencer\n";
            strcpy(msg.messageText, message);
            printf ("Message que le père encode : %s; Code du message : %d\n",msg.messageText,msg.code);

            for(int i= 0 ; i<nbPlayers; i++){
                swrite(pipes[i].pipefdWrite[1],&msg,sizeof(StructMessage));
            } 

            if(argc<3 ){
                tilesbag = createTiles();
            }else{
                tilesbag = (int*) malloc(NUMBER_OF_TILES*sizeof(int));
                if (tilesbag ==NULL)
                {
                    perror("tilesbag error allocation");
                    exit(1);
                }

                for (int i = 0; i < NUMBER_OF_TILES; ++i){
                    tilesbag[i] = tilesTemp[nextTileTemp];
                    nextTileTemp++;
                }

            } 
            
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
                strcpy(tableJoueursIPC[i].pseudo ,tabPlayers[i].pseudo);
                tableJoueursIPC[i].score = tabPlayers[i].score;
            }

            msg.code= RANKING;
            for (int i = 0; i < nbPlayers; ++i){
                swrite(pipes[i].pipefdWrite[1],&msg,sizeof(msg));
                swrite(pipes[i].pipefdWrite[1],&nbPlayers,sizeof(int));
                swrite(pipes[i].pipefdWrite[1],&tableJoueursIPC,sizeof(Structplayer));
                printf("nbPlayers pere : %d",nbPlayers);
            }

            //ON SORT DE LA ZONE CRITIQUE
            sem_up0 (semID); 

            msg.code = END_GAME;
            for (int i = 0; i < nbPlayers; ++i){
                swrite(pipes[i].pipefdWrite[1],&msg,sizeof(msg));
                printf("message.code ENDGAME %d\n", msg.code);
            }

            for (int i = 0; i < nbPlayers; ++i)
            {
                swait(&fils[i]);
            }        
        

            freeAllMemory(shmId,semID,tableJoueursIPC);
            freeAllPipes(pipes,nbPlayers);
            freeAllSocket(tabPlayers,nbPlayers);
            freeAllMalloc(tabPlayers,fils);


        }    



        }

            ssigprocmask(SIG_UNBLOCK,&blocked,NULL);
        }

    free(tilesbag);
    exit(0);

} 










