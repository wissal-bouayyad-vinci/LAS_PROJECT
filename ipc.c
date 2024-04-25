#include "ipc.h"
#include "utils_v1.h"
#include  "config.h"

int* initSharedMemory(){
    int shm_id = sshmget(SHM_KEY, sizeof(int), IPC_CREAT | PERM);
    int* z = sshmat(shm_id);

    return z;
}



int* getsharedMemory(){
    int shm_id = sshmget(SHM_KEY, sizeof(int), 0);
    int* z = sshmat(shm_id);

    return z;
}