#include<stdio.h>
#include<sys/msg.h>

#include"../includes/MessageTool.h"
const char* PATH_NAME = ".";

int main(){
    int id = GetMessageQueueID(PATH_NAME);
    char content[128];
    int size;
    while( 1 ){
        // 不等待
        ReadMessage( id, content, &size, 111, IPC_NOWAIT );
        if( size == -1 )    break;

        printf("Read %d bytes: %s\n", size, content);
    }
    return 0;
}