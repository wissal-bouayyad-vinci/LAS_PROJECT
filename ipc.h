#ifndef IPC_H
#define IPC_H 

/**
 * PRE: 
 * POST: on success, you get the shared memory pointer.
 * RES:  return a pointer to the shared memory.
 */
int* initSharedMemory();


/**
 * PRE:   
 * POST: on success, you get the shared memory pointer.
 * RES:  return a pointer to the shared memory.
 */
int* getsharedMemory();

#endif
