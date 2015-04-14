#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void * thread_handler(void * arg)
{
    int *a = (int *)arg;

    printf("tid:%lu s arg = %d\n", pthread_self(), *a);

    return NULL;
}

int main(void)
{
    int ret;
    pthread_t tid[5];
    int i, k;

    for (i = 0; i < 5; i++)
    {
        ret = pthread_create(tid + i, NULL, thread_handler, (void *)&i);
        if (ret)
        {
            fprintf(stderr, "pthread_create:%s\n", strerror(ret));
            exit(1);
        }
        sleep(1);
    }

    for (k = 0; k < 5; k++)
    {
        pthread_join(tid[k], NULL);
    }

    exit(0);
}
