#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>


void * thread(void *arg)
{
    pthread_t tid = pthread_self();

    printf("tid = %lu\n", tid);
    char * p = (char *)malloc(20);
    sprintf(p, "%lu", tid);
    printf("p--step1__tid %s\n", p);

    sleep(6);
    printf("tid = %lu\n", tid);
    printf("p--step2__tid %s\n", p);
    return NULL;
}

int main(void)
{   
    int i = 0;
    pthread_t tid[2];
    
    for (i; i < 2; i++)
    {
        pthread_create(&tid[i], NULL, thread, NULL);
        if (i == 1)
            sleep(3);
    }
    sleep(10);
    return 0;
}
