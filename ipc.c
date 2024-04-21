#include "ipc.h"
#include "utils_v1.h"

// met int key ou direct appel au config.h
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