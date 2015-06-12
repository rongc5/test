#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdio.h>

int a[100];

void init_array(int x)
{
    int i;
    srand(pthread_self());
    for (i = 0; i < 100; i++)
    {
        a[i] = i + x;
        usleep(rand()%1000 + 200);
    }
}

void display_array(void)
{
    int i;
    for (i = 0; i < 100; i++)
    {
        printf("a[%d] = %3d  ", i, a[i]);
        if (i % 5 == 0)
            printf("\n");
    }
}

void * thread_handler(void *arg)
{
    init_array(0);
    pthread_exit(NULL);
}

int main(void)
{
    pthread_t tid;
    int ret;

    ret = pthread_create(&tid, NULL, thread_handler, NULL);
    if (ret)
    {
        fprintf(stderr, "pthread:%s\n", strerror(ret));
        exit(1);
    }

    init_array(5);
    pthread_join(tid, NULL);
    display_array();
    pthread_exit(NULL);
}
