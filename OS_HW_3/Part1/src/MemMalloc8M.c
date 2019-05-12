#include<stdio.h>
#include<malloc.h>

#define KB ( 1024 )
#define MB ( KB * 1024 )

void *bufs[5];
int main(){
    printf("efore malloc()s.\n");
    getchar();
    
    bufs[0] = malloc( MB * 8 );
    bufs[1] = malloc( MB * 8 );
    bufs[2] = malloc( MB * 8 );
    bufs[3] = malloc( MB * 8 );
    bufs[4] = malloc( MB * 8 );

    printf("After 5 malloc()s.\n");
    getchar();

    free(bufs[1]);
    bufs[1] = NULL;
    free(bufs[3]);
    bufs[3] = NULL;

    printf("After 2 free()s.\n");
    getchar();

    for( int i = 0 ; i < 5 ; i++ ){
        if( bufs[i] ){
            free(bufs[i]);
        }
    }
    return 0;
}