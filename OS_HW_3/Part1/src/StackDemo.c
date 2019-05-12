#include<stdio.h>

void upAndDown(int n){
    printf("Level %d: n location %p\n", n, &n);
    getchar();
    if( n < 3 ){
        upAndDown(n+1);
    }
    printf("Level %d: n location %p\n", n, &n);
    getchar();
}

int main(){
    upAndDown(1);
    return 0;
}