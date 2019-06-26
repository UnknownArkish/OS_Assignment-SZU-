
#include<stdio.h>
#include<malloc.h>
#include<unistd.h>
#include<sys/types.h>

#define KB ( 1024 )
#define MB ( 1024 * KB )
#define GB ( 1024 * MB )

#define MinMallocB ( 4 * KB )
#define MaxMallocB ( 1 * GB )

#define MaxMallocCount 1000000
void* mems[MaxMallocCount];
unsigned long sumB = 0;

// 清理程序
void Clean(){
    for( int i = 0; i < MaxMallocCount; i++ ){
        if( mems[i] != NULL ){
            free(mems[i]);
            mems[i] = NULL;
        }
    }
}

int ToTest( int index, unsigned int toMallocB ){
    if( toMallocB < MinMallocB ){
        return index;
    }
    if( index >= MaxMallocCount ){
        return -1;
    }
    // 申请内存
    mems[index] = malloc(toMallocB);
    if( mems[index] != NULL ){              // 如果成功读取则继续
        // 统计
        sumB += toMallocB;
        return ToTest( index+1, toMallocB );
    }else{                                  // 如果申请内存失败，则折半需要申请的内存
        return ToTest( index, toMallocB / 2 );
    }
}
int ToTest_(){
    int index = 0;
    unsigned int toMallocB = MaxMallocB;
    while( toMallocB >= MinMallocB ){
        if( index >= MaxMallocCount )   return -1;
        mems[index] = malloc(toMallocB);
        if( mems[index] != NULL ){
            sumB += toMallocB;
            index++;
        }else{
            free(mems[index]);  mems[index] = NULL;
            toMallocB /= 2;
        }
    }
    return index;
}


int main(){
    // int result = ToTest( 0, MaxMallocB );
    int result = ToTest_();
    Clean();
    if( result >= 0 ){
        // 进行修正
        sumB += ( MaxMallocCount * sizeof(void*) + sizeof(unsigned long) );
        printf("MaxB: %u\n", sumB);
        printf("MaxKB: %u\n", sumB / KB);
        printf("MaxMB: %u\n", sumB / MB);
        printf("MaxGB: %u\n", sumB / GB);
    }else{
        printf("error on index");
    }
    return 0;
}
