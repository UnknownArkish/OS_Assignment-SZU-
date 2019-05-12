#include<stdio.h>
#include<string.h>

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