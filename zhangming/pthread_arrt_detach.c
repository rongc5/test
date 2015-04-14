#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void * thread_handler(void * arg)
{
    sleep(5);
    printf("thread exit!\n");
    return NULL;
}

int main(void)
{
    pthread_t tid;
    pthread_attr_t attr;
    int ret, status;

    pthread_attr_init(&attr);

    pthread_attr_getdetachstate(&attr, &status);
    if (status == PTHREAD_CREATE_JOINABLE)
    printf("default is joinable!\n");
    else if (status == PTHREAD_CREATE_DETACHED)
    printf("default is detached!\n");

    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    ret = pthread_create(&tid, &attr, thread_handler, NULL);
    if (ret)
    {
        fprintf(stderr, "pthread_create:%s", strerror(ret));
        exit(1);
    }

    ret = pthread_join(tid, NULL);
    if (ret)
    {
        printf("join:%s\n", strerror(ret));
    }
    pthread_exit(NULL);
}
