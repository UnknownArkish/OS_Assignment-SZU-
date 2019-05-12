#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>

int main(){
    pid_t pid;
    int fds[2];                                     
    char buf[128];
    int nwr = 0;

    pipe(fds);                                    // 打开两个文件描述符
    pid = fork();
    if( pid < 0 ){
        printf("Error with fork\n");
        exit(1);
    }else if( pid == 0 ){
        printf("This is child process, pid = %d\n", getpid());
        printf("Child: waiting for message...\n");
        close(fds[1]);                              // 子进程关闭管道的写端
        nwr = read(fds[0], buf, sizeof(buf));
        printf("Child: receive %d bytes from parent: \"%s\"\n", nwr, buf);
        exit(0);
    }else{
        printf("This is parent process, pid = %d\n", getpid());
        printf("Parent: sending message...\n");
        close(fds[0]);                              // 父进程关闭管道的读端
        strcpy(buf, "Message from parent!");
        nwr = write(fds[1], buf, strlen(buf)+1);
        printf("Parent: send %d bytes to child\n", nwr);
        wait(pid);
        exit(0);
    }
}