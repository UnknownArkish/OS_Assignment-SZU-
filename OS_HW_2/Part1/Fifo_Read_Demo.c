#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>

const char* FIFO_PATH_NAME = "MY_FIFO_FIFE";

int main(){
    // 如果FIFO不存在则创建一个FIFO文件
    mkfifo(FIFO_PATH_NAME, O_CREAT|0666);

    // 打开FIFO文件
    int fifo_fd = open(FIFO_PATH_NAME,O_RDWR);
    if( fifo_fd == -1 ){
        fprintf(stderr, "error with open\n");
        exit(2);
    }
    // 缓存区，准备从FIFO中读取数据
    char buffs[128];
    ssize_t size = read(fifo_fd, buffs, sizeof(buffs));
    printf("Read %u size from FIFO: %s\n",strlen(buffs), buffs);
    // 关闭FIFO文件
    close(fifo_fd);
    return 0;
}