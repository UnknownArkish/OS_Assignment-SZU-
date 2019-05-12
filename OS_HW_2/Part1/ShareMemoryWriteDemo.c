#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>

#define SHARE_MEMORY_SIZE 4096

const char* PATH_NAME = ".";

int main(int argc, char** argv){
    // 得到共享内存的id
    int key = ftok(PATH_NAME,'m');
    int id = shmget(key, SHARE_MEMORY_SIZE, IPC_CREAT|0666 );
    // 将共享内存映射到进程空间
    char* buffers;
    buffers = shmat(id, 0, 0 );
    if( buffers < (char*)0 ){
        fprintf(stderr, "Error with shmat\n");
        exit(1);
    }

    printf("Attached to share momery at: %p\n", buffers);
    system("ipcs -m");

    // 查看进程空间地址的变化
    char syslog[128];
    sprintf(syslog, "cat /proc/%d/maps",getpid());
    system(syslog);
    // 将数据写入共享内存中
    char* message = argv[1];
    strcpy( buffers, message );
    // 与共享内存断开连接
    if( shmdt(buffers) < 0 ){
        fprintf(stderr,"Error with shmdt\n");
        exit(2);
    }

    printf("Detached to share momery\n");
    system("ipcs -m");

    return 0;
}