
#include<stdio.h>
#include<string.h>
#include"util.h"

int GetContent( char buf[], int offset, int size, int endian ){
    int result = 0;
    if( endian == LITTLE_ENDIAN ){
        for( int i = 0 ; i < size ; i++ ){
            result += ( ( buf[i + offset] & 0xff )  << (i << 3) );
        }
    }else if( endian == BIG_ENDIAN ){
        for( int i = 0 ; i < size ; i++ ){
            result += ( ( buf[i + offset] & 0xff )  << ((size-i) << 3));
        }
    }
    return result;
}

int Split(char** result, char* input, const char* delim){
	char *pNext;
	int count = 0;
	if (input == NULL || strlen(input) == 0)
		return 0;
	if (delim == NULL || strlen(delim) == 0)
		return 0;
	pNext = strtok(input, delim);
	while (pNext != NULL) {
		strcpy(result[count],pNext);	
		++count;
		pNext = strtok(NULL, delim);
	}
	return count;
}