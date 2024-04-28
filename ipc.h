#ifndef IPC_H
#define IPC_H 

#include "player.h"

#define PERM 0666
#define SEM_KEY  3
#define SHM_KEY  4

/**
 * PRE: nbrPlayers: the number of players we need to keep space for in the memory zone
 * POST: on success, you get the shared memory pointer.
 * RES:  return a pointer to the shared memory.
 */
int initSharedMemory(int nbrPlayers);

/**
 * 
 */ 
Structplayer* attacheSHM(int shmID);

/**
 * PRE:   
 * POST: on success, you get the shared memory pointer.
 * RES:  return a pointer to the shared memory.
 */
Structplayer* getsharedMemory(int nbrPlayers);

/**
 * PRE:
 * POST: on success,you get the semaphore id 
 * RES: return the semaphore's id  
 */
int initSemaphore();

/**
 * PRE:
 * POST: on success, you get the semaphore id
 * RES: return the semaphore's id
*/
int getSemaphore();

#endif
