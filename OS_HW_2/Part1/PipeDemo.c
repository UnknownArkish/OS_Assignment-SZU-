#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>

int main(){
    pid_t pid = 0;
    int pds[2];
    // 缓存区
    char buffs[128];
    // 打开一个匿名管道
    pipe(pds);
    pid = fork();
    // 判断进程，以区分父子进程的不同行为
    if( pid < 0 ){
        printf("Fork Error!\n");
        return -1;
    }else if( pid == 0 ){
        printf("This is child process, pid: %d\n",getpid());
        close(pds[1]);     // 子进程关闭匿名管道的输出流
        int size = read( pds[0],buffs, sizeof(buffs) );
        printf("Read %d size from parent: %s\n",size, buffs);
        exit(0);
    }else{
        printf("This is parent process, pid: %d\n",getpid());
        printf("Parent sending message...\n");
        close(pds[0]);      // 父进程关闭匿名管道的输入流
        scanf("%s",buffs);
        write(pds[1],buffs,strlen(buffs));
        wait(pid);
    }
    
    return 0;
}