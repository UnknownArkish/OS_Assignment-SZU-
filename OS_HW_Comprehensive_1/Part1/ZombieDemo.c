#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int main(){
    pid_t pid = fork();
    if( pid < 0 ){
        printf("Error!\n");
    }else if( pid == 0 ){
        printf("Hi father! I am a ZOMBIE\n");
        exit(0);
    }else{
        getchar();
        wait(NULL);
    }
    return 0;
}