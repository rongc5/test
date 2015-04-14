#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_attr_t attr;
void * thread(void *arg)
{
    size_t size;

    pthread_attr_getstacksize(&attr, &size);

    printf("size= %u\n", size);
    
    return NULL;
}

int main(void)
{
    pthread_t tid;
    size_t size=1000*1000;
    pthread_attr_init(&attr);
    pthread_attr_getstacksize(&attr, &size);
    
    printf("defult threadstack size = %li\n", size);
    size=1000*1000;

    pthread_attr_setstacksize(&attr, size);

    pthread_create(&tid, &attr, thread, NULL);

    sleep(1);
    return 0;
}
