#ifndef IPC_H
#define IPC_H 

/**
 * PRE: key is the key to create a shared memory.
 * POST: on success, you get the shared memory pointer.
 * RES:  return a pointer to the shared memory.
 */
int* initSharedMemory(int key);


/**
 * PRE: key is the key to the existing shared memory.
 * 
 * POST: on success, you get the shared memory pointer.
 * RES:  return a pointer to the shared memory.
 */
int* getsharedMemory(int key);

#endif
