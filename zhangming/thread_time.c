#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void * thread_handler(void *arg)
{
    return NULL;
}
int main(void)
{
    int i;
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    for (i = 0; i < 10000; i++)
    {
        pthread_create(&tid, &attr, thread_handler, NULL);
        pthread_join(tid,NULL);
    }

    //pthread_join(tid,NULL);
}
