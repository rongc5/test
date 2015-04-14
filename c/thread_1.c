#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *thread(void *arg)
{
    while (1)
    {
        int i=0;
        printf("%p\n", &i);
        sleep(1);
    }
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, thread, NULL);

    while (1)
        sleep(99);

    return 0;
}
