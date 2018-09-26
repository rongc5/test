#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int c, char **v)
{
	if (c != 2){
		printf("usage...\n");
		return -1;
	}

	printf("%d\n", atoi(v[1]));

    char * ptr = "\t 12345\t333";

	printf("%d\n", atoi(ptr));

	return 0;
}
