#include<stdio.h>
#include<string.h>

#include"../includes/MessageTool.h"

const char* OUTPUT_MESSAGE = "Hello Message Queue!";
const char* PATH_NAME = ".";

int main(int argc, char** argv){
    char* message = argv[1];
    // 获得PATH_NAME对应的消息队列ID
    int id = GetMessageQueueID(PATH_NAME);
    // 向消息队列中写入数据，type设置为111
    SendMessage(id, message, strlen(message)+1, 111 );
    return 0;
}