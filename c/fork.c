#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h>   

int main(void) 
{    
	int i;    
	for(i=0; i<2; i++)
	{       
		fork();       
		//printf("1");    
        printf("%d, %d, %d\n", getpid(), getppid(), i);
	}      
	return 0; 
}
