#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

int g_val = 0;
char *g_ptr;
int *p;

void * thread_handler(void * arg)
{
    int x = 8;
    p = &x;
    sleep(3);
    printf("tid:%lu s arg = %d\n, %d, %s\n", pthread_self(), getpid(), x, g_ptr);

    //     pthread_exit(NULL);
    return NULL;
}

int main(void)
{
    int ret;
    pthread_t tid;

    g_ptr = (char *)malloc(20);
    if (NULL == g_ptr)
    {
        perror("malloc");
        exit(1);
    }

    strcpy(g_ptr, "hello world!");

    ret = pthread_create(&tid, NULL, thread_handler, NULL);
    if (ret)
    {
        fprintf(stderr, "pthread_create:%s\n", strerror(ret));
        exit(1);
    }
    sleep(1);
    *p = 10;
    pthread_join(tid, NULL);

    printf("join success!\n");
    // pthread_exit(NULL);
    exit(0);
}
