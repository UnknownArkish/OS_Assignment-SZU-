#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/sem.h>
#include<sys/ipc.h>

union semun{
    int             val;
    struct semid_ds *buf;
    unsigned short  *array;
};

int main(int argc, char** argv){
    int semid, nsems, i;
    struct semid_ds seminfo;
    unsigned short* ptr;
    union semun arg;
    if( argc != 2 ){
        printf("usage: dlete_sems <pathname>");
        exit(0);
    }
    semid = semget(ftok(argv[1], 0), 0, 0);
    arg.buf = &seminfo;

    int ret = semctl(semid, 0, IPC_RMID, NULL);
    if( ret == -1 ){
        printf("delete sem error\n");
        exit(1);
    }
    exit(0);
}