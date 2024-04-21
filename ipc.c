#include "ipc.h"

int* initSharedMemory(int key){
    int shm_id = sshmget(key, sizeof(int), IPC_CREAT | PERM);
    int* z = sshmat(shm_id);

    return z;
}



int* getsharedMemory(int key){
    int shm_id = sshmget(key, sizeof(int), 0);
    int* z = sshmat(shm_id);

    return z;
}