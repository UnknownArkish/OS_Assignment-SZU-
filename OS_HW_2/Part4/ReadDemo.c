#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<string.h>
#include<pthread.h>
#include <semaphore.h>

#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)

#include"../includes/SystemVIPCTool.h"

#define SHARE_MEMORY_SIZE 4096

const char* SemFile = "MySemFile";
const char* ShareMemoryFile = "MyShareMemoryFile";

int semID;
int shmID;
char * shmBuf;

// 初始化信号量
void InitSem(){
    printf("Created System V IPC sem...\n");
    // 申请两个内核级别的信号量
    // 0信号量为缓冲区空
    // 1信号量为缓冲区满
    semID = GetSemID(SemFile, 2, IPC_CREAT | 0666);
    if( semID < 0 ){
        fprintf(stderr,"Errpr woth GetSemid\n");
        exit(1);
    }
}
// 初始化共享内存
void InitShareMemory(){
    int key = ftok(ShareMemoryFile, 'm');
    shmID = shmget(key, SHARE_MEMORY_SIZE,IPC_CREAT | 0666);
    if( shmID < 0 ){
        fprintf(stderr,"Errpr woth shmID\n");
        exit(1);
    }
}

// int posixSemID;
sem_t posixSem;

// 创建用于线程互斥的所，初始值为1
void InitPosixSem(){
    printf("Created posix sem...\n");
    sem_init(&posixSem, 0, 1);
}

void CleanShareMemory(){
    printf("Empty share memory...\n");
    memset(shmBuf, 0, SHARE_MEMORY_SIZE);
}

int shouldExit = 0;
int* ReadTask(void* arg){
    printf("Thread %d enter function ReadTask...\n",gettid());

    while( 1 ){
        // 线程间抢占互斥锁
        sem_wait(&posixSem);
        printf("tid:%u is ready to read share memory...\n", gettid());
        if( shouldExit ){
            sem_post(&posixSem);
            break;
        }

        // 申请缓冲区为满的信号量
        SemPAt(semID, 1, 1, 0 );
        printf("Read from share memory: %s\n",shmBuf);
        if( strcmp(shmBuf,"exit") == 0 ){
            shouldExit = 1;
        }
        CleanShareMemory();
        // 通知共享内存空了
        SemVAt(semID, 0, 1, 0);
    
        // 释放线程互斥锁
        printf("tid:%u done the task...\n\n", gettid());
        sem_post(&posixSem);
        sleep(1);
    }
    printf("%d thread exit\n", gettid());
}

#define THREAD_NUM 2


int main(){
    InitSem();
    InitShareMemory();
    InitPosixSem();

    // 映射到共享内存
    shmBuf = (char*)shmat(shmID, 0, 0);
    if( shmBuf < (char*)0 ){
        fprintf(stderr,"Errpr woth shmat\n");
        exit(1);
    }    

    // 创建线程，使他们执行ReadTask函数
    int tids[THREAD_NUM];                       // 线程id
    pthread_t threads[THREAD_NUM];              // 线程号
    int i;
    for( i = 0 ; i < THREAD_NUM ; i++ ){
        if( pthread_create( &threads[i], NULL, (void*)ReadTask, &tids[i] )){
            fprintf(stderr, "Error with pthread_create\n");
        }
    }
    printf("\n");
    // 将线程参与调度，并等到所有线程结束
    for( i = 0 ; i < THREAD_NUM; i++ ){
        pthread_join( threads[i], NULL );
    }

    // 关闭互斥锁
    printf("Destory posix sem...\n");
    sem_destroy(&posixSem);

    // 解除与共享内存的映射
    if( shmdt(shmBuf) < 0){
        fprintf(stderr,"Errpr woth shmdt\n");
        exit(1);
    }

    // 删除SystemVIPC的锁
    printf("Delete System V IPC...\n");
    DeleteSem(semID);
    return 0;
}
