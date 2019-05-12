#include<sempahore.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>

int main(int argc, char** argv){
    if( argc != 2 ){
        printf("Please input the ID of sem!\n");
        exit(1);
    }
    sem_unlink(argv[1]);
    return 0;
}