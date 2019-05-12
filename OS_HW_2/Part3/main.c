#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<string.h>
#include<sys/msg.h>

#include"../includes/MessageTool.h"

const char* PATH_NAME = ".";

int id;

// B进程调用的函数，主要是调用三次SendMessage
void BTask(){
    const char* message1 = "The 111 message";
    const char* message2 = "The 222 message";
    const char* message3 = "The 333 message";

    SendMessage(id, message1, strlen(message1)+1, 111);
    SendMessage(id, message2, strlen(message2)+1, 222);
    SendMessage(id, message3, strlen(message3)+1, 333);
}
void DTask(){
    RemoveMessageQueue(id);
    exit(0);
}
void CTask(){
    char buffer[128];
    int size;
    // 以阻塞的形式从消息队列中分别读取类型为333/111/222的消息
    ReadMessage(id,buffer,&size,333,0);
    printf("Read %d bytes from type 333: %s\n", size, buffer);
    ReadMessage(id,buffer,&size,111,0);
    printf("Read %d bytes from type 111: %s\n", size, buffer);
    ReadMessage(id,buffer,&size,222,0);
    printf("Read %d bytes from type 222: %s\n", size, buffer);

    int pidD = fork();
    if( pidD < 0 ){
        fprintf(stderr, "Error with fork D");
        exit(1);
    }else if( pidD == 0 ){
        printf("D Process created...\n");                   // C进程读取完后，创建D进程
        DTask();
        exit(0);
    }else{
        wait(pidD);
        exit(0);
    }
}

int main(){
    id = GetMessageQueueID(PATH_NAME);                      // A进程创建一个消息队列，此id是全局变量
    pid_t pidB = fork();
    if( pidB < 0 ){
        fprintf(stderr, "Error with fork B\n");
        exit(1);
    }else if( pidB == 0 ){
        printf("B Process created...\n");                   // B进程负责向消息队列中写入数据
        BTask();
        exit(0);
    }else if(pidB > 0){
        pid_t pidC = fork();
        if( pidC < 0 ){
            fprintf(stderr, "Error with fork C\n");
            exit(1);
        }else if(pidC == 0){
            printf("C Process created...\n");               // C进程负责从消息队列中读取数据
            CTask();
            exit(0);
        }else
        {
            // 等待B和C进程结束
            wait(pidB);
            wait(pidC);
            exit(0);
        }
    }
    return 0;
}