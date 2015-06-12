#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

static i = 0;
char *p[3];
void * thread(void *arg)
{
	char q;
	char c;
#if 1
	if (i == 0){
	    c=9;
	    i++;
	}
#endif
	pthread_t tid = pthread_self();
	//printf("tid = %lu\n", tid);
	printf("c:%p	q:%p\n", &c, &q);

	while (1)
		sleep(9999);
	return NULL;
}

int main(void)
{   
	int i = 0;
	pthread_t tid[10];

	pthread_attr_t attr;
	size_t stacksize = 2048*1024;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (pthread_attr_setstacksize(&attr, stacksize))
		printf("failed!\n");
	for (i; i < 10; i++)
	{
		pthread_create(&tid[i], &attr, thread, NULL);
		if (i == 5)
			sleep(9);
		sleep(5);
		printf("thread:%p\n", thread);
	}

	while (1)
		sleep(100000);
	return 0;
}
