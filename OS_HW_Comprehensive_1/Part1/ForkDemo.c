#include<stdio.h>
#include<unistd.h>

int main(int argc, char** argv){
    int pid = fork();
    if( pid < 0 ){
        printf("Error!\n");
    }else if( pid == 0 ){
        printf("This is the child process!\n");
        getchar();
    }else{
        printf("This is the parent process! child process id = %d\n", pid);
        getchar();
        printf("Parent process exit...\n");
    }
    return 0;
}