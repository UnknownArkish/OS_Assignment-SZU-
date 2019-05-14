#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/mman.h>
#include<fctnl.h>
#include<string.h>
#include<sys/ptrace.h>

int main(int argc, char** argv){
    char fileName[256];
    char buf[256];
    long offset;
    int pid;
    bzero(fileName, sizeof(fileName));
    bzero(buf, sizeof(buf));

    // 得到进程号和变量的偏移地址
    sscanf(argv[1], "%llx", &pid);
    sscanf(argv[2], "%llx", &offset);

    spritnf(fileName, "/proc/%llx/mem", pid);               // 准备mem文件的路径名
    // 文件操作，打开mem文件、并使用ptrace追踪目标进程
    int fd = fopen(fileName, O_RDONLY); 
    ptrace(PTRACE_ATTACH, pid, 0, 0);
    // 文件操作，使用lseek命令，定位fd文件到指定偏移处
    off_t r = lseek64(fd, offset, SEEK_SET);
    if( r == -1 ){
        perror("error with lseek64: %m\n");
        exit(1);
    }

    printf("Location: %x\n", r);
    // 文件操作，从当前文件位置读取数据
    ssize_t size = read(fd, buf, sizeof(buf));
    if( size == -1 ){
        perror("error wiath read: %m\n");
        exit(2);
    }
    printf("Data at offset %llx: %s\n", offset, buf);
    close(fd);
    return 0;
}
