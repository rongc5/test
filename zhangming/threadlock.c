#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pthread_mutex_t lock_a = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_b = PTHREAD_MUTEX_INITIALIZER;

void * thread_handler(void * arg)
{
    pthread_mutex_lock(&lock_b);
    printf("I have get the b lock(branch)!\n");
    sleep(5);
    printf("I will lock a(branch)!\n");
    pthread_mutex_lock(&lock_a);
    printf("I have got the a lock(branch)!\n");
    pthread_mutex_unlock(&lock_a);
    pthread_mutex_unlock(&lock_b);

    return NULL;
}

int main(void)
{
    pthread_t tid;
    int ret;

    ret = pthread_create(&tid, NULL, thread_handler, NULL);
    if (ret)
    {
        fprintf(stderr, "pthread_create:%s\n", strerror(ret));
        exit(1);
    }

    pthread_mutex_lock(&lock_a);
    printf("I have get the a lock!\n");
    sleep(5);
    printf("I am waitting b lock!\n");
    pthread_mutex_lock(&lock_b);
    printf("I have get the b lock!\n");
    pthread_mutex_unlock(&lock_b);
    printf("I have released the b lock!\n");
    pthread_mutex_unlock(&lock_a);

    pthread_exit(NULL);
}
