#include<stdio.h>
#include<unistd.h>

int main(){

    int pid = fork();
    if( pid < 0 ){
        printf("Error\n");
    }else if( pid == 0 ){
        printf("This is the child process!\n");
        fork();
        getchar();
    }else{
        printf("This is hte parent process! child process id = %d\n", pid);
        fork();
        getchar();        
    }
    return 0;
}