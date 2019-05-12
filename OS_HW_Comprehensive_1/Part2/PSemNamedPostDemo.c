#include<semaphore.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>

int main(int argc, char** argv){
    sem_t *sem;
    int val;

    if( argc != 2 ){
        printf("Please input a file name\n");
        exit(1);
    }
    sem = sem_open(argv[1], 0);
    sem_post(sem);
    sem_getvalue(sem, &val);
    printf("value = %d\n", val);
    return 0;
}