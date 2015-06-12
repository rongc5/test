#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void * thread_handler(void * arg)
{
    return NULL;
}

int main(void)
{
    pthread_attr_t attr;
    pthread_t tid;
    int i, ret;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    for (i = 0; i < 10000; i++)
    {
        ret = pthread_create(&tid, &attr, thread_handler, NULL);
        if (ret)
        {
            fprintf(stderr, "create:%s\n", strerror(ret));
            exit(1);
        }
    }

    pthread_exit(NULL);
}
