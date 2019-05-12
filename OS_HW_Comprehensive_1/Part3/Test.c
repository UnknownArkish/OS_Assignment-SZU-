#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(){

    int pfd[2];
    pipe(pfd);

    pid_t pid = fork();
    if( pid < 0 ){
    }else if( pid == 0 ){
        dup2( pfd[0], 0 );      // 拷贝至标准输入
        close(pfd[1]);          // 关闭输出
        char* argv[] = {NULL};
        execvp("more", argv);
    }else{
        dup2(pfd[1], 1);        // 拷贝至标准输出
        close(pfd[0]);          // 关闭输入
        char* argv[] = {"-l", NULL};
        execvp("ls", argv);
    }
    return 0;
}