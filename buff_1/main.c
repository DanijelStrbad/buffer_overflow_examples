#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	int i, N = 10;
	char msgBuff[4], fileName[100] = "file123.txt";
	
	printf("&msgBuff[0]  = %p\n", (void *) msgBuff);
	printf("&fileName[0] = %p\n", (void *) fileName);

	for(i=0; i<N; i++) {
		msgBuff[i] = 'a';
	}
	msgBuff[N] = (char)0;

	printf("%s\n\n", fileName);

	return 0;
}
