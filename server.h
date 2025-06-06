#ifndef SERVER_H
#define SERVER_H

#include "player.h"

// VARIABLES USED IN SERVER
#define MAX_PLAYERS 3
#define MIN_PLAYERS 2
#define TIME_INSCRIPTION 15
#define NUMBER_OF_PLAYS 20
#define NUMBER_OF_TILES 40
#define NUMBER_OF_USED_TILES 20
#define SCORE_TABLE_SIZE 3
#define TOTAL_TILES 40

// CREATION DES PIPES POUR LA COMMUNCATION ENTRE PERE ET FILS
typedef struct structPipe
{
    int pipefdRead[2];
    int pipefdWrite[2];
} structPipe;

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

int *createTiles();

/**
 * PRE: tilebag: It is a Integer table with 40 randomly displayed tiles.
 *      nextTile: It is the index we use to dig the next tile in the tilebag.
 * POST: increments the nextTile, so the program knows which tile to dig next,
 *       once it is called again.
 * RES: returns the digged tile.
 */
int digTile(int *tilebag, int *nextTile);

/**
 * PRE: Player: The new player we are going to put in the table of players
 *      nbPlayers: number of Players already in the table
 * POST: The playersTable will contain a new player.
 *       The number of players will be incremented, due to the new add.
 * RES: /
 */
void addPlayerToTable(Structplayer *tableauDesJoueurs, Structplayer newPlayer, int *nbPlayers);

/**
 * PRE:  players: The table of players that is going to be sorted
 *        nbPlayers: The number of players present in the table
 * POST: The players table will be sorted by descending score
 * RES:  /
 */
void sortByScoreDescending(Structplayer *players, int nbPlayers);

/**
 * PRE: shmId : id of the shared memory.
 *      sem_id : id of the semaphore
 *      tableJoueursIPC : pointer to the shared memory
 *
 */
void freeAllMemory(int shmId, int sem_id, Structplayer *tableJoueursIPC);

/**
 * PRE: pipes : table that contains every child's pipes
 *      nbPlayers : number of players
 *
 *      closes the descriptors end
 */
void freeAllPipes(structPipe *pipes, int nbPlayers);

/**
 * PRE: tabPlayers : table that contains every player
 *      nbPlayers : number of players
 *
 *      closes the sockets
 */
void freeAllSocket(Structplayer *tabPlayers, int nbPlayers);

/**
 * PRE: tabPlayers : table that contains every player
 *      fils : table that contains every child's id
 *      tilesbag : table that contains the tiles
 *
 *      frees the allocated memory
 */
void freeAllMalloc(Structplayer *tabPlayers, int *fils, int *tilesbag);

/**
 * PRE: table is a table of  strings
 * RES:the number of lines from the table
 */
size_t getTableSize(char **table);

#endif