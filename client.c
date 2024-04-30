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
 

size_t getTableSize(char **table){

    size_t size =0;

    if (table)
    {
        while(table[size]!=NULL){
            size++;
            
        }
    }

    return size;

}


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


   
    //RECUPERER LE FICHIER PLACEMENTS
    int fdPlacements;
    char** tablePlacements;
    int* placesChosen;
    if(argc == 3){

        
        fdPlacements = sopen(argv[2], O_RDONLY, 0444);
        tablePlacements = readFileToTable(fdPlacements);
        
        strcpy(pseudoPlayer ,tablePlacements[0]);
        printf("PSEUDO :  %s\n ",pseudoPlayer);
        printf("PSEUDO %s \n", tablePlacements[0]);

        placesChosen  = (int*) malloc(MAX_NUMBER_TURN*sizeof(int));
        if(placesChosen==NULL){
            perror("error allocation placesChosen");
            exit(1);
        } 
        
        for(int i=1;i<MAX_NUMBER_TURN;i++) {  
           placesChosen[i] = atoi(tablePlacements[i]); 
        } 

    } 




    // recup

    printf("Bienvenu(e)!. Inscrivez-vous pour commencer le jeu.\n");
    if (placesChosen ==NULL ){
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
    sread(sockfd,&message,sizeof(message));


    if (message.code == START_GAME){
        printf("START GAME\n");

         
        int* grille =  initGrille();

        for (int i = 1; i <=MAX_NUMBER_TURN; i++){ 
            sread(sockfd,&message,sizeof(message));
    
            if (message.code==NUMERO_TUILE){
                printf("------------\n ");
                tileNumber = message.tuile;
                printf("Veuillez choisir un emplacement :  pour la tuile %d \n", tileNumber);
                // printf("tileNumber : %d\n", tileNumber);
                printGrille(grille);


                int chosenPlacement;

                if (argc==3)
                {

                    chosenPlacement = placesChosen[prochainPlacement];
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
                swrite(sockfd,&message,sizeof(message));

            }else{
                printf("Réponse Serveur: non prevue %d.\n", message.code);
            }
        }



        // on a fini le jeux 

        // envoie de MON SCORE
        message.code=MON_SCORE;
        swrite(sockfd,&message,sizeof(message));
        printf("message.code ; attentdu : MONSCORE : %d\n ", message.code);
        int scoreFinal = scoreCalculation(grille);
        int score=scoreFinal;
        swrite(sockfd,&score,sizeof(int));
        printf("scoreFinael : %d", score);

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
            
 
        // end         
        sread(sockfd,&message,sizeof(message));
        if(message.code == END_GAME){
            printf("END GAME\n");
            free(grille);
            free(players);
            sclose(sockfd);
        } 
            
    }else if(message.code == CANCEL_GAME){
        printf("Jeu annulé : il manque des joueurs\n");
        sclose(sockfd);
    }
 
    return 0;

}
 