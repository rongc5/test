#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void * thread_handler(void * arg)
{
    printf("%lu was created, and my pid is %d\n", pthread_self(), getpid());

    return NULL;
}

int main(void)
{
    int ret;
    pthread_t tid;

    ret = pthread_create(&tid, NULL, thread_handler, NULL);
    if (ret)
    {
        fprintf(stderr, "pthread_creat:%s\n", strerror(ret));
        exit(1);
    }
    sleep(1);
    exit(0);
}
