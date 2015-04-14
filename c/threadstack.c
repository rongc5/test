#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

pthread_attr_t attr;
static i = 0;
char *p[3];
void * thread(void *arg)
{
    char buf1[10];
    char buf2[20]; 
    char buf3[30];
    size_t stacksize;
    pthread_t tid = pthread_self();
    if (i == 0) {
    	//p[0] = (char *)malloc(10);
    	memcpy(buf1, "hello", 10);
	i++;
    }else if (i == 1) {
    	//p[1] = (char *)malloc(20);
    	memcpy(buf2, "hello world!", 20);
	i++;
    }else if (i == 2) {
	//p[2] = (char *)malloc(30);
	memcpy(buf3, "hello world, haier", 30);
    }
    printf("tid = %lu\n", tid);
    
    pthread_attr_getstacksize(&attr, &stacksize);
    printf("stacksize = %lu\n", stacksize);

    while (1)
    sleep(9999);
    return NULL;
}

int main(void)
{   
    int i = 0;
    pthread_t tid[3];
    
    size_t stacksize = 1024*16;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (pthread_attr_setstacksize(&attr, stacksize))
	    printf("failed!\n");
    for (i; i < 3; i++)
    {
        pthread_create(&tid[i], &attr, thread, NULL);
	sleep(5);
        if (i == 5)
            sleep(9);
    }

    while (1)
        sleep(100000);
    return 0;
}
