#ifndef PLAYER
#define PLAYER 

#define SERVER_PORT 9501
#define SERVER_IP "127.0.0.1" /* localhost */
#define MAX_PSEUDO 256

/* struct player used between server and client*/
typedef struct 
{
    char pseudo[MAX_PSEUDO];
    int score;
    
}Structplayer;


#endif