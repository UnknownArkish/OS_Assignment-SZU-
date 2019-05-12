#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/sem.h>
#include<sys/ipc.h>

int main(int argc, char** argv){
    int c, i, flag, semid, nops;
    struct sembuf* ptr;
    flag = 0;
    while( ( c = getopt( argc, argv, "nu" ) ) != -1 ){
        switch (c)
        {
            case 'n':
                flag |= IPC_NOWAIT;
                break;a
            case 'u':
                flag |= SEM_UNDO;
                break;
        }
    }
    if( argc - optind < 2 ){
        printf("usage: semops [-u] [-n] <pathname> opration...\n");
        exit(0);
    }
    if( ( semid = semget(ftok(argv[optind], 0), 0, 0) ) == -1 ){
        perror("semget() error\n");
        exit(-1);
    }
    optind++;
    nops = argc - optind;
    ptr = calloc( nops, sizeof(struct sembuf) );
    for( i = 0; i < nops; i++ ){
        ptr[i].sem_num = i;
        ptr[i].sem_op = atoi(argv[optind+i]);
        ptr[i].sem_flg = flag;
    }
    if( semop(semid, ptr, nops) == -1 ){
        perror("semop() error\n");
        exit(-1);
    }
    exit(0);
}