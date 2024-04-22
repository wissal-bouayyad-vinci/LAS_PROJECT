#ifndef PLAYER
#define PLAYER 

#define MAX_PSEUDO 256

/* struct player used between server and client*/
typedef struct {
    char pseudo[MAX_PSEUDO];
    int score;
} Structplayer;

#endif
