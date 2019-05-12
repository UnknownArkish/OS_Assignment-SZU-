#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/sem.h>
#include<sys/ipc.h>

#define SEM_R 0400
#define SEM_A 0200
#define SVSEM_MODE (SEM_R | SEM_A | SEM_R >> 3 | SEM_R >> 6)

int main(int argc, char** argv){
    int c, oflag, semid, nsems;
    oflag = SVSEM_MODE | IPC_CREAT;
    while( (c = getopt(argc, argv, "e")) != -1 ){
        switch(c){
            case 'e':
                oflag |= IPC_EXCL;
                break;
        }
    }
    if( optind != argc - 2 ){
        printf("usage: semcreate [-e] <pathname> <nsems>");
        exit(0);
    }
    nsems = atoi(argv[optind+1]);
    semid = semget(ftok(argv[optind], 0), nsems, oflag);
    return 0;
}