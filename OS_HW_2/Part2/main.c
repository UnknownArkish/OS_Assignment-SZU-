#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>

int main( int argc, char** argv ){
    char* message = argv[1];
    
    int pds_AB[2];
    pipe(pds_AB);
    pid_t pidB = fork();
    if( pidB < 0 ){
        fprintf(stderr,"Error with fork B\n");
        exit(1);
    }else if( pidB == 0 ){
        close(pds_AB[1]);
        char buffers[128];
        // B进程从匿名管道的输出端读取消息
        ssize_t size = read(pds_AB[0], buffers, sizeof(buffers));
        printf("B read %u bytes from A: %s\n", size, buffers);

        // B进程产生C进程，并使用匿名管道进行通信
        // 与AB间通信类似
        int pds_BC[2];
        pipe(pds_BC);
        pid_t pidC = fork();
        if( pidC < 0 ){
            fprintf(stderr,"Error with fork C\n");
            exit(1);
        }else if( pidC == 0 ){
            close(pds_BC[1]);
            char buffers_[128];
            ssize_t size_ = read(pds_BC[0],buffers_, sizeof(buffers_));
            printf("C read %u bytes from B: %s\n", size_, buffers_);
        }else{
            close(pds_BC[0]);
            write(pds_BC[1],buffers,size);
        }
    }else{
        // A进程向匿名管道的输入端写入消息
        close(pds_AB[0]);
        write(pds_AB[1],message,strlen(message)+1);
    }


    return 0;
}