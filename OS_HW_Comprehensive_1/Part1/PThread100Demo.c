#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

void* thread(void* argv){
    printf("This is a pthread.\n");
    sleep(10);
}
int main(int argc, char** argv){
    pthread_t tid[100];
    int i, ret;
    for( i = 0 ; i < 100 ; i++ ){
        ret = pthread_create( &tid[0], NULL, thread, NULL );
        if( ret < 0 ){
            printf("Create pthread error\n");
        }
    }
    printf("This is the main process.\n");
    for( i = 0 ; i < 100 ; i++ ){
        pthread_join(tid[i], NULL);
    }
    return 0;
}