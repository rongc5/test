#include <stdio.h>
#include <string.h>

int main(int c, char **v)
{
	if (c != 2){
		printf("usage...\n");
		return -1;
	}

	printf("%d\n", atoi(v[1]));

	return 0;
}
