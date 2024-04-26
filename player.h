#ifndef PLAYER
#define PLAYER 

#define MAX_PSEUDO 256

/* struct player used between server and client*/
typedef struct Structplayer {
    char pseudo[MAX_PSEUDO];
    int sockfd;
    int score;
    int sockfd;
} Structplayer;

#endif
