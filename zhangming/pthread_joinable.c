#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void * thread_handler(void *arg)
{
    sleep(3);
    int * ret = (int *)malloc(sizeof(int));

    printf("%lu was created, and my pid is %d\n", pthread_self(), getpid());
    *ret = 8;
    return ret;
}

int main(void)
{
    int r;
    pthread_t tid;
    void *ret;

    r = pthread_create(&tid, NULL, thread_handler, NULL);
    if (r)
    {
        fprintf(stderr, "pthread_create: %s\n", strerror(r));
        exit(1);
    }

    pthread_join(tid, &ret);

    printf("join success!, ret = %d\n", *((int *)ret));
    free(ret);
    return 0;
}

