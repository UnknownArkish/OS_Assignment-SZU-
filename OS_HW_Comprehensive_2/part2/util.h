

#define LITTLE_ENDIAN 0
#define BIG_ENDIAN 1


int GetContent( char buf[], int offset, int size, int endian );

int Split(char** result, char* input, const char* delim);