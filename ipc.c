#include "ipc.h"
#include "utils_v1.h"
#include  "network_serveur.h"
#include "player.h"



int initSharedMemory(int nbPlayers){
    int shm_id = sshmget(SHM_KEY, nbPlayers*sizeof(Structplayer), IPC_CREAT | PERM);
    return shm_id;
}


Structplayer* attacheSHM(int shmID){
    Structplayer* z = sshmat(shmID);
    return z; 
}



Structplayer* getsharedMemory(int nbPlayers){
    int shm_id = sshmget(SHM_KEY, nbPlayers*sizeof(Structplayer), 0);
    Structplayer* z = sshmat(shm_id);

    return z;
}


int initSemaphore(){
    int idSem = sem_create(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | PERM, 1);
    return idSem;
} 


int getSemaphore(){
    int idSem = sem_get(SEM_KEY,1);
    return idSem;
} 


