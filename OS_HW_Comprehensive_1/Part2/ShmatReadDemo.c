#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int main(int argc, char** argv){
    int shm_id;
    char* shm_buf;

    if( argc != 2 ){
        printf("USAGE: atshm <identifier>\n");
        exit(1);
    }

    shm_id = atoi(argv[1]);
    if( (shm_buf = shmat(shm_id, 0, 0)) < (char*)0 ){
        perror("shmat fail!\n");
        exit(1);
    }

    printf("Segment attached at %p\n", shm_buf);
    system("ipcs -m");
    printf("The string in SHM is: %s\n", shm_buf);
    getchar();

    if( shmdt(shm_buf) < 0 ){
        perror("shmdt fail!\n");
        exit(1);
    }
    printf("Segment detached\n");
    system("ipcs -m");

    getchar();
    return 0;
}