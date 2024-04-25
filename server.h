#ifndef SERVER_H
#define SERVER_H

#define MAX_PLAYERS 3
#define TIME_INSCRIPTION 30
#define NUMBER_OF_PLAYS 20
#define NUMBER_OF_TILES 40
#define NUMBER_OF_USED_TILES 20
#define SCORE_TABLE_SIZE 3
#define TOTAL_TILES 40

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

int* createTiles()

/**
 * PRE: tilebag: It is a Integer table with 40 randomly displayed tiles. 
 *      nextTile: It is the index we use to dig the next tile in the tilebag. 
 * POST: increments the nextTile, so the program knows which tile to dig next,
 *       once it is called again.
 * RES: returns the digged tile.
 */ 
int digTile(int* tilebag, int* nextTile);

/**
 * PRE: fds: structure that contains every fd of every signed in player
 *      nbPlayers: number of Players in the game
 * POST: /
 * RES: returns a table of players
 */
void Structplayer* createPlayersTable (struct pollfd fds, int nbPlayers);



char envoyerTuile();

void trierRankingParScore();

void makeRanking(adresse ipc);





#endif