
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