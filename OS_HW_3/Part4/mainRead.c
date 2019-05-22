
#include<stdio.h>
#include<malloc.h>
#include<unistd.h>
#include<sys/types.h>

#define KB ( 1024 )
#define MB ( 1024 * KB )
#define GB ( 1024 * MB )

#define MallocB (256 * MB)
#define MallocCount ( MallocB / sizeof(char) )

#define ReadPerB (4 * KB)
#define ReadPerIndex ( ReadPerB / sizeof(char) )

char* mems;
char status[128];

//  初始化程序
void Init(){
    mems = (char*)malloc( MallocB );

    pid_t pid = getpid();
    sprintf(status, "/proc/%d/status", pid);
}
// 清理程序
void Clean(){
    if( mems != NULL ){
        free(mems);
        mems = NULL;
    }
}

void LogStatus( char* msg = "" ){
    printf("Status::%s\n", msg);
    system(status);
}

void ToReadAndCheckStatus(){
    char temp;
    for( int i = 0 ; i < MallocCount; i += ReadPerIndex ){
        temp = mems[i];
    }   
}
void ToWriteAndCheckStatus(){
    for( int i = 0 ; i < MallocCount; i += ReadPerIndex ){
        mems[i] = i % 256;
    }   
}

int main(){
    Init();

    LogStatus( "Before Read" );
    ToReadAndCheckStatus();
    LogStatus( "After Read" );

    Clean();
    return 0;
}