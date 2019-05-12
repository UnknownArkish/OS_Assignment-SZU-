#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>

const char* FIFO_PATH_NAME = "MY_FIFO_FIFE";
const char* MESSAGE_OUTPUT = "Hello,FIFO!";

int main(){
    // 如果FIFO文件不存在，进行创建、否则不作任何处理
    mkfifo(FIFO_PATH_NAME, O_CREAT|0666);

    // 打开FIFO文件
    int fifo_fd = open(FIFO_PATH_NAME,O_RDWR);
    if( fifo_fd == -1 ){
        fprintf(stderr, "error with open\n");
        exit(2);
    }
    // 对FIFO文件进行写入
    write( fifo_fd, MESSAGE_OUTPUT, strlen(MESSAGE_OUTPUT)+1 );
    // 关闭FIFO文件
    close(fifo_fd);
    return 0;
}