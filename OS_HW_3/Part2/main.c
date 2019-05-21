
#include<stdio.h>
#include<malloc.h>
#include<unistd.h>
#include<sys/types.h>

#define KB ( 1024 * 1 )
#define MB ( 1024 * KB )

#define Count 6
void* mems[Count+1];

char maps[128];
char status[128];

// 初始化程序
void Init(){
    pid_t pid = getpid();
    sprintf(maps, "/proc/%d/maps", pid);
    sprintf(status, "/proc/%d/status", pid);
    for( int i = 0; i <= Count; i++ ){
        mems[i] = NULL;
    }
}
// 清理程序
void Clean(){
    for( int i = 1 ; i <= Count ; i++ ){
        if( mems[i] != NULL ){
            free(mems[i]);
            mems[i] = NULL;
        }
    }
}

void LogMaps( char* msg = "" ){
    printf("Maps::%s\n", msg);
    system(maps);
}

void LogStatus( char* msg = "" ){
    printf("Status::%s\n", msg);
    system(status);
}

int main(){
    Init();

    LogMaps("Beginning");
    LogStatus("Beginning");

    for( int i = 1 ; i <= Count ; i++ ){
        mems[i] = malloc( MB * 128 );
        printf("#%d malloc: %px\n", mems[i]);
    }
    LogMaps( "After malloc 6 times 128MB" );
    LogStatus("After malloc 6 times 128MB");

    // 释放2、3、5号空间
    free( mems[2] );    mems[2] = NULL;
    free( mems[3] );    mems[3] = NULL;
    free( mems[5] );    mems[5] = NULL;
    LogMaps( "After free 2, 3, 5" );
    LogStatus("After malloc 2, 3, 5");

    // 申请1024MB
    mems[0] = malloc( MB * 1024 );
    LogMaps( "After malloc 1024MB" );
    LogStatus("After malloc 1024MB");

    void* 64mb = malloc( MB * 64 );
    LogMaps( "After malloc 64MB" );
    LogStatus("After malloc 64MB");

    // 清理内存
    free(64mb); 64mb = NULL;
    Clean();
    return 0;
}