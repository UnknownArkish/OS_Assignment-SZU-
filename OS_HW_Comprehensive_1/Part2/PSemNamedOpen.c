#include<semaphore.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>

int main(int argc, char** argv){
    sem_t* sem;

    if( argc != 2 ){
        printf("Please input a file name to act as the ID of the sem!\n");
        exit(1);
    }
    sem = sem_open(argv[1], O_CREAT, 0644, 1);
    exit(0);
}