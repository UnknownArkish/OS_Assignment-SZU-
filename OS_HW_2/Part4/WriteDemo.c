#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>

#include"../includes/SystemVIPCTool.h"

#define SHARE_MEMORY_SIZE 4096

const char* SemFile = "MySemFile";
const char* ShareMemoryFile = "MyShareMemoryFile";

int semID;
int shmID;

// 初始化信号量
void InitSem(){
    // 申请两个内核级别的信号量
    // 0信号量为缓冲区空
    // 1信号量为缓冲区满
    semID = GetSemID(SemFile, 2, IPC_CREAT | 0666);
    if( semID < 0 ){
        fprintf(stderr,"Errpr woth GetSemid\n");
        exit(1);
    }
    // 初始缓冲区为空，所以第一个值为1，第二个值为0
    short inilValues[] = {1,0};
    SetAllValue(semID, inilValues);
}
// 初始化共享内存
void InitShareMemory(){
    int key = ftok(ShareMemoryFile, 'm');
    shmID = shmget(key, SHARE_MEMORY_SIZE,IPC_CREAT | 0666);
    if( shmID < 0 ){
        fprintf(stderr,"Errpr woth shmID\n");
        exit(1);
    }

    int semNum = GetSemNum(semID);
    short *values = malloc(semNum*sizeof(short));
    GetAllValues(semID,values);
    int i;
    for( i = 0 ; i < semNum ; i++ ){
        printf("%d\n",values[i]);
    }
}

int main(){
    InitSem();
    InitShareMemory();

    // 映射到共享内存
    char* buf = shmat(shmID, 0, 0);
    if( buf < (char*)0 ){
        fprintf(stderr,"Errpr woth shmat\n");
        exit(1);
    }
    char input[128];
    int shouldExit = 0;
    while(1){
        if(shouldExit) break;

        // 申请编号为0的资源
        SemPAt(semID, 0, 1, 0 );
        // 读取键盘，并将输入写入至共享内存
        scanf("%s", input);
        if(strcmp(input,"exit") == 0){
            shouldExit = 1;
        }
        memcpy(buf, input, strlen(input)+1);
        // 通知缓冲区已满
        SemVAt(semID, 1, 1, 0);
    }
    // 解除与共享内存的映射
    if( shmdt(buf) < 0){
        fprintf(stderr,"Errpr woth shmdt\n");
        exit(1);
    }

    return 0;
}
