#include<stdio.h>

int main(){
    char *str = "HelloWorld";
    printf("%s @ %p\n", str, str);
    getchar();
    return 0;
}