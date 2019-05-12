#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<stdio.h>

#define BUFSIZE 4096

int main(int argc, char** argv){
    int shm_id;

    shm_id = shmget(IPC_PRIVATE, BUFSIZE, 0666);
    if( shm_id < 0 ){
        perror("shmget fail!\n");
        exit(1);
    }

    printf("Successfully created segment: %d\n", shm_id);
    system("ipcs -m");

    return 0;
}