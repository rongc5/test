#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

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
        if ((s + BASE)%i == 0)
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
    while (1)
    {
        pthread_mutex_lock(&g_lock);
        pthread_cond_wait(&g_cond, &g_lock);
        do_prime(g_val);
        pthread_mutex_unlock(&g_lock);
    }
}

int main(void)
{
    int ret, k;
    pthread_t tid;

   ret = pthread_create(&tid, NULL, thread_handler, NULL);
   if (ret)
   {
       fprintf(stderr, "pthread:%s\n", strerror(ret));
       exit(1);
   }
   sleep(1);

   while (1)
   {
       printf("请输入数据: ");
       fflush(stdout);
       scanf("%d", &k);
       pthread_mutex_lock(&g_lock);
       g_val = k;
       pthread_mutex_unlock(&g_lock);
       pthread_cond_signal(&g_cond);
       //usleep(1000);
   }

   return 0;
}
