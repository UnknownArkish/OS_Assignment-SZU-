#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/errno.h>
#include<string.h>

#include"../includes/MessageTool.h"

// 以当前路径作为key的创建
const char* PathName = ".";

// int GetMessageQueueID(){
//     key_t key = ftok(PathName,'m');
//     int id = msgget(key, IPC_CREAT | 0660);
//     return id;
// }
int GetMessageQueueID(const char* pathName){
    key_t key = ftok(pathName,'m');
    int id = msgget(key, IPC_CREAT | 0660);
    return id;
}
int CheckMessageQueueIDValid(int id){
    if( id < 0 ){
        fprintf(stderr,"Error with msgget\n");
        return -1;
    }
    return 0;
}

void SendMessage( int id, const char* content, int n, long type){
    // int id = GetMessageQueueID();
    if( CheckMessageQueueIDValid(id) < 0 ){
        exit(1);
    }
    struct MessageBuffer buffer;
    memcpy(buffer.message, content, n);
    buffer.messageType = type;
    if( msgsnd(id, &buffer, n, 0) == -1){
        fprintf(stderr,"Error with msgsnd: %d\n",errno);
        exit(2);
    }
    system("ipcs -q");
}
void ReadMessage( int id, char* content, int* n, long type, int msgFlag){
    // int id = GetMessageQueueID();
    if( CheckMessageQueueIDValid(id) < 0 ){
        exit(1);
    }
    struct MessageBuffer buffer;
    
    buffer.messageType = type;
    ssize_t size = msgrcv( id, &buffer, MAXIMUM_MESSSAGE_SIZE, type, msgFlag );
    *n = size;
    if( size == -1 )    return;

    memcpy(content, buffer.message, size);
    system("ipcs -q");
}
void RemoveMessageQueue( int id ){
    // int id = GetMessageQueueID();
    if( CheckMessageQueueIDValid(id) < 0 ){
        exit(1);
    }
    msgctl(id, IPC_RMID, 0);
}