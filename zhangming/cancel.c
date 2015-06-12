#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void * thread_handler(void * arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

    unsigned long long int i = 100000000;
    while (i--)
    {
        i++;
        i--;
        pthread_testcancel();
    }

    printf("over!\n");
}

int main(void)
{
    pthread_t tid;
    int ret;

    ret = pthread_create(&tid, NULL, thread_handler, NULL);
    if (ret)
    {
        fprintf(stderr, "pthread_create: %s\n", strerror(ret));
        exit(1);
    }

    sleep(2);

    pthread_cancel(tid);

    pthread_exit(NULL);
}
