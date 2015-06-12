#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define BASE 300000000
typedef long long unsigned int ull_t;
int g_val = 0;
pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t g_cond = PTHREAD_COND_INITIALIZER;

void do_prime(int s)
{
    ull_t i;
    int flag = 0;

    for (i = 2; i < s + BASE; i++)
    {
        if ((s + BASE) % i == 0)
        {
            flag = 1;
            break;
        }
    }

    if (1 == flag)
        printf("\n%llu is not a prime!\n", (ull_t)(s + BASE));
    else
        printf("\n%llu is a prime!\n", (ull_t)(s + BASE));
}

void * thread_handler(void * arg)
{
    int tmp;
    while (1)
    {
        pthread_mutex_lock(&g_lock);
        printf("tid %lu wait!\n", pthread_self());
        pthread_cond_wait(&g_cond, &g_lock);
        printf("I am %lu!\n", pthread_self());
      //  sleep(3);
        pthread_mutex_unlock(&g_lock);
    }
}

int main(void)
{
    int k, ret;
    pthread_t tid;
    int i;

    for (i = 0; i < 5; i++)
    {
        ret = pthread_create(&tid, NULL, thread_handler, NULL);
        if (ret)
        {
            fprintf(stderr, "pthread: %s\n", strerror(ret));
            exit(1);
        }
    }

    sleep(1);
    while (1)
    {
        printf("输入数据: ");
        fflush(stdout);
        scanf("%d", &k);
        pthread_cond_signal(&g_cond);
    }
    return 0;
}
