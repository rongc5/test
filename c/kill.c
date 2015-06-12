#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>


int main(int c, char **v)
{
	if (c != 2){
		printf("usage...\n");
	}

	kill(atol(v[1]), 9);

	return 0;
}
