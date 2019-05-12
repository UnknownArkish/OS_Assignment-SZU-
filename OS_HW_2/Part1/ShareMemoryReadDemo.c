#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>

#define SHARE_MEMORY_SIZE 4096

const char* PATH_NAME = ".";

int main(){
    // 得到共享内存的id
    int key = ftok(PATH_NAME,'m');
    int id = shmget(key, SHARE_MEMORY_SIZE, IPC_CREAT|0666 );

    char* buffers;
    // 将共享内存映射到进程空间
    buffers = shmat(id, 0, 0 );
    if( buffers < (char*)0 ){
        fprintf(stderr, "Error with shmat\n");
        exit(1);
    }
    // 从共享内存中读取数据
    printf("Read from share momery: %s\n", buffers);
    // 与共享内存断开连接
    if( shmdt(buffers) < 0 ){
        fprintf(stderr,"Error with shmdt\n");
        exit(2);
    }
    return 0;
}