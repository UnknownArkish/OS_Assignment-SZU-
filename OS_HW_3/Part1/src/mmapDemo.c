#include<fcntl.h>
#include<unistd.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<stdio.h>
#include<string.h>

int main(){
    int fd;
    void* start_addr;
    struct stat sb;
    char str1[] = "Modifacation in the memory-------------";
    fd = open("file_mapped.txt", O_RDWR);                       // 打开文件
    fstat(fd, &sb);                                             // 获取文件属性
    // 与文件内容进行映射
    start_addr = mmap( NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0 );
    if( start_addr == MAP_FAILED ){
        perror("error with mmap\n");
    }
    printf("Mapped area started by addr: %llx:\n%s\n", (long long unsigned int)start_addr, (char*)start_addr);
    getchar();
    strcpy( start_addr, str1 );
    printf("Write string into the mapped area!\n");
    getchar();
    // 解除映射
    munmap(start_addr, sb.st_size);
    close(fd);
    return 0;
}