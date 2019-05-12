#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>

#include"../includes/SystemVIPCTool.h"

int main(){
    int id = GetSemID(".", 3, IPC_CREAT | 0666);
    DeleteSem(id);
    // int semNum = GetSemNum(id);
    // short* values = malloc(semNum * sizeof(short));

    // int i;
    // for( i = 0 ; i < semNum ; i++ ){
    //     values[i] = i+1;
    // }
    // SetAllValue(id, values);
    // for( i = 0 ; i < semNum ; i++ ){
    //     values[i] = 0;
    // }

    // GetAllValues(id, values);
    // for( i = 0 ; i < semNum ; i++ ){
    //     printf("[%d]:%d\n",i, values[i]);
    // }

    // free(values);
    return 0;
}

// 创建/获取一个信号量集ID
int GetSemID(const char* pathName, int nsems, int flag){
    int key = ftok(pathName, 'm');
    int id = semget(key, nsems, flag);
    return id;
}
// 获取制定信号量集的semid_ds数据结构
int GetState(int id, struct semid_ds* semInfo ){
    union SemUnion arg;
    arg.buf = semInfo;
    return semctl(id, 0, IPC_STAT, arg);
}
// 得到信号量集中的信号量个数
int GetSemNum(int id){
    struct semid_ds semInfo;
    GetState(id, &semInfo);
    return semInfo.sem_nsems;
}
// 获取指定信号量的value
int GetValue(int id, int semIndex){
    union SemUnion arg;
    if( semctl(id, semIndex, GETVAL, arg) < 0)  return -1;
    return arg.val;
}
// 获取所有信号量的value，结果放置在buf中
int GetAllValues(int id, short* buf){
    int semNum = GetSemNum(id);
    if( semNum < 0 )    return -1;

    union SemUnion arg;
    arg.array =  (unsigned short*)malloc(semNum * sizeof(unsigned short));
    if( semctl(id, 0, GETALL, arg)) return -1;

    int i;
    for( i = 0 ; i < semNum ; i++ ){
        buf[i] = arg.array[i];
    }
    // 防止内存泄漏
    free(arg.array);
}
// 设置指定信号量的value
int SetValue(int id, int semIndex, int value){
    union SemUnion arg;
    arg.val = value;
    return semctl(id, semIndex, SETVAL, arg);
}
// 使用values设置所有信号量的value
int SetAllValue(int id, short* values){
    int semNum = GetSemNum(id);
    union SemUnion arg;
    arg.array =  (unsigned short*)malloc(semNum * sizeof(unsigned short));
    int i;
    for( i = 0 ; i < semNum ; i++ ){
        arg.array[i] = values[i];
    }
    int ctlResult = semctl(id, 0, SETALL, arg);
    // 防止内存泄漏
    free(arg.array);
    return ctlResult;
}
// 对信号量集进行P操作，申请资源
int SemP(int id, struct sembuf* sops, unsigned nsops){
    int i;
    for( i = 0 ; i < nsops ; i++ ){
        sops[i].sem_op = -fabs(sops[i].sem_op);
    }
    return semop(id, sops, nsops);
}
// 对单个信号量进行P操作
int SemPAt(int id, int semIndex, int pValue, int flag){
    struct  sembuf buf;
    buf.sem_num = semIndex;
    buf.sem_op = pValue;
    buf.sem_flg = flag;
    SemP(id, &buf, 1);
}
// 对信号量集进行V操作
int SemV(int id, struct sembuf* sops, unsigned nsops){
    int i;
    for( i = 0 ; i < nsops ; i++ ){
        sops[i].sem_op = fabs(sops[i].sem_op);
    }
    return semop(id, sops, nsops);
}
// 对单个信号量进行V操作
int SemVAt(int id, int semIndex, int pValue, int flag){
    struct  sembuf buf;
    buf.sem_num = semIndex;
    buf.sem_op = pValue;
    buf.sem_flg = flag;
    SemV(id, &buf, 1);
}
// 删除信号量集
int DeleteSem(int id){
    return semctl(id, 0, IPC_RMID, NULL);
}