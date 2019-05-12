#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

void task(){
    int thread_var = 0xff;
    printf("This a pthread, thread_var's addr:%llx\n", &thread_var);
    sleep(10);
}

int main(void){
    pthread_t id1 ,id2;
    int i, ret;
    getchar();
    ret = pthread_create(&id1, NULL, (void*)task, NULL);
    if( ret != 0 ){
        perror("Error with pthread_create\n");
        exit(1);
    }
    ret = pthread_create(&id2, NULL, (void*)task, NULL);
    if( ret != 0 ){
        perror("Error with pthread_create\n");
        exit(1);
    }
    
    printf("This is the main process. i addr: %llx\n", &i);
    pthread_join(id1, NULL);
    pthread_join(id2, NULL);
    return 0;
}