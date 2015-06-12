#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pthread_mutex_t lock;
int g_val = 8;

void test1(void)
{
    pthread_mutex_lock(&lock);
    g_val++;
    pthread_mutex_unlock(&lock);
}

void test0(void)
{
    test1();
}

void * thread_handler(void * arg)
{
    pthread_mutex_lock(&lock);
    test0();
    printf("lock ok!\n");
    pthread_mutex_unlock(&lock);
}

int main(void)
{
    pthread_t tid;
    int ret;
    pthread_mutexattr_t mattr;

    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutex_init(&lock, &mattr);

    ret = pthread_create(&tid, NULL, thread_handler, NULL);
    if (ret)
        exit(1);
    pthread_join(tid, NULL);
    printf("%d\n", g_val);
    pthread_exit(NULL);
}
