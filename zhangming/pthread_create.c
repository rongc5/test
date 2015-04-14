#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void * thread_handler(void * arg)
{
    printf("tid:%lu s arg = %d\n", pthread_self(), getpid());

    return NULL;
}

int main(void)
{
    int ret;
    pthread_t tid;

        ret = pthread_create(&tid, NULL, thread_handler, NULL);
        if (ret)
        {
            fprintf(stderr, "pthread_create:%s\n", strerror(ret));
            exit(1);
        }
       pthread_join(tid, NULL);

    exit(0);
}
