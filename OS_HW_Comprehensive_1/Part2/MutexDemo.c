#include<pthread.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

#define THREAD_NUM 16
#define MB 1024 * 1024

int *array;
int length;
int count;
int t;

pthread_mutexattr_t m;

int* count3s_thread(void* id){
    int length_per_thread = length / THREAD_NUM;
    int start = *(int*)id * length_per_thread;
    int i;
    for( i = start; i < start + length_per_thread ; i++ ){
        if( array[i] == 3 ){
            pthread_mutex_lock(&m);
            count++;
            pthread_mutex_unlock(&m);
        }
    }
}

int main(int argc, char** argv){
    int i;
    int tid[THREAD_NUM];
    pthread_t threads[THREAD_NUM];
    length = 64 * MB;
    array = malloc(length * sizeof(int));
    pthread_mutex_init(&m, NULL);
    
    for( i = 0; i < length; i++ ){
        array[i] = i % 2 ? 4 : 3;
    }
    count = 0;
    for( t = 0 ; t < THREAD_NUM ; t++ ){
        tid[t] = t;
        int err = pthread_create( &threads[t], NULL, (void*)count3s_thread, &tid[t] );
        if( err ){
            printf("cread thread error!\n");
            exit(1);
        }
    }
    for( t = 0 ; t < THREAD_NUM; t++ ){
        pthread_join( threads[t], NULL );
    }
    printf("Total count = %d\n", count);
    return 0;
}