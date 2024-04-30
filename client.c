#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <stdlib.h>
#include "player.h"
#include "game.h"
#include "network_client.h"
#include "message.h"
#include "utils_v1.h"
#include "config.h"
#include "client.h"
 
void printGrille(int* grille){
    printf("GRILLE DE JEUX\n");
    printf("%2s ", "");
    for (int i = 0; i < GRILLE_SIZE; ++i)
    {
        printf("%3d ", (i+1));
    }
    printf("\n");
    printf("%2s ", "");
    for (int i = 0; i < GRILLE_SIZE; ++i)
    {
        if (grille[i]==-1)
        {
            printf("%3s ", "-");
        }else if (grille[i]==31)
        {
            printf("%3s ", "Jo");
        }else{
            printf("%3d ",grille[i]);
        }
    }
    printf("\n");
}
 
void printRanking( Structplayer* players,int size){
    printf("RANKING FINAL\n");
    for (int i = 0; i < size; ++i)
    {
        printf("%d. %s avec un score de %d\n",(i+1),players[i].pseudo, players[i].score);
    }
}
 
int main(int argc, char const *argv[])
{
    if(argc < 2){
        perror("Missing port");
        exit(1);
    }
    int SERVER_PORT = atoi(argv[1]);

    

    // attributs : 
    int prochainPlacement=0;
    Structplayer* players;
    char pseudoPlayer[MAX_PSEUDO];
    int sockfd;
    int ret;
    int tileNumber;
    StructMessage message;


    FILE *fdPlacements;
    //RECUPERER LE FICHIER PLACEMENTS
    int * placements;
    if(argc == 3){

        fdPlacements = fopen(argv[2],"r");    
        if (fdPlacements==NULL){
            perror("le dossier donner dans les arguments est invalid");
            exit(1);   
        }else{
            placements= malloc (MAX_NUMBER_TURN * sizeof(int));
            if(!placements){
                    perror("ALLOCATION ERROR");
                    exit(1);
            }

            int cptPlaces = 0;

            fscanf(fdPlacements, "%s", pseudoPlayer);    

            while(fscanf(fdPlacements, "%d", &placements[cptPlaces])==1){
                cptPlaces++;
            } 
        }
    } 




    // recup

    printf("Bienvenu(e)!. Inscrivez-vous pour commencer le jeu.\n");
    if (argc != 3 ){
        printf("Nom: \n");
        ret = sread(0,pseudoPlayer,MAX_PSEUDO);
        pseudoPlayer[ret - 1] = '\0';
    }

    strcpy(message.messageText,pseudoPlayer);
    message.code = INSCRIPTION_REQUEST;
 
    // start socket
    sockfd = initSocketClient(SERVER_IP,SERVER_PORT);
    swrite(sockfd,&message,sizeof(message));
 
    /* wait server response */
    sread(sockfd, &message, sizeof(message));
 
    switch(message.code){   
        case INSCRIPTION_OK:
            printf("Réponse Serveur: INSCRIPTION acceptée.\n");
            break;

        case INSCRIPTION_KO:
            printf("Réponse Serveur: INSCRIPTION refusée.\n");
            break;

        default:
            printf("Réponse Serveur: non prevue %d.\n", message.code);
            break;
    }
 
   /*wait to know if the game START oR CANCEL*/
    if(sread(sockfd,&message,sizeof(message))){
        printf("message code 1 : %d\n ",message.code);
        printf("message 2: %s \n ", message.messageText);
    }else{
        printf("mais je read rien en fait");
    }
    printf("Message recu du serveur fils : %s ; Le code reçcu du serveur fils: %d\n",message.messageText,message.code );

    if (message.code == START_GAME){
        printf("START GAME\n");

        // pseudoPlayer code : 
        int* grille =  initGrille();

        for (int i = 1; i <=20; i++){ 
            sread(sockfd,&message,sizeof(message));

            if(message.code==CANCEL_GAME){
                printf("CANCEL_GAME\n");
                sclose(sockfd);
            } 
            else if (message.code==NUMERO_TUILE){
                printf("------------\n ");
                tileNumber = message.tuile;
                printf("Veuillez choisir un emplacement :  pour la tuile %d \n", tileNumber);
                // printf("tileNumber : %d\n", tileNumber);
                printGrille(grille);


                int chosenPlacement;

                if (argc==3)
                {
                    chosenPlacement = placements[prochainPlacement];
                    printf("chosenPlacement : %d\n", chosenPlacement);
                    prochainPlacement++;
                    choosePlacement(tileNumber,chosenPlacement,grille);
                }else{

                    char input[256];
                    fgets(input, sizeof(input), stdin);
                    sscanf(input, "%d", &chosenPlacement);

                    while(!choosePlacement(tileNumber, chosenPlacement, grille)){    
                        char input[256];
                        fgets(input, sizeof(input), stdin);
                        sscanf(input, "%d", &chosenPlacement);
                    }
                }

                // if true
                printf("\n");
                printf("\n");
                printGrille(grille);
                message.code = PLACEMENT_TERMINE;
                ret = swrite(sockfd,&message,sizeof(message));
                //printf("Voici vos placement : \n");

            }else{
                printf("Réponse Serveur: non prevue %d.\n", message.code);
            }
        }



        // on a fini le jeux 
        int scoreFinal = scoreCalculation(grille);
        int score=scoreFinal;
        swrite(sockfd,&score,sizeof(int));
        

        // attend message ranking
        sread(sockfd,&message,sizeof(message));

        if ((message.code==RANKING)){
            int size;   
            sread(sockfd,&size,sizeof(size));
            
            players =  malloc (size * sizeof(Structplayer));
            if(!players){
                perror("ALLOCATION ERROR");
                exit(1);
            }

            for (int i = 0; i < size; ++i)
            {
                Structplayer newplayer;
                sread(sockfd,&newplayer,sizeof(Structplayer));
                strcpy(players[i].pseudo,newplayer.pseudo);
                players[i].score = newplayer.score;
                
            }
            printRanking(players,size);

        }else{
            printf("Réponse Serveur: non prevue %d.\n", message.code);                
        }
            
 
        //  end         
        sread(sockfd,&message,sizeof(message));
        if(message.code == END_GAME){
            printf("END GAME\n");
            free(grille);
            free(players);
            sclose(sockfd);
        } 
            
    }else{
        printf("Jeu annulé : il manque des joueurs\n");
        sclose(sockfd);
    }
 
    return 0;

}
 
