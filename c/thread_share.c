#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

char *p;
void * thread(void *arg)
{
    static i = 0;

    char buf[33];
    int a;
    pthread_t tid = pthread_self();
    printf("tid = %lu\n", tid);
    
    if (0 == i) {
        p = (char *)malloc(10);
        memcpy(p, "hello", 10);
        i++;
        memcpy(buf, "world!", 10);
        a = 9;
    }else {
        printf("p = %s buf%s, a= %d\n", p, buf, a);
    }


    return NULL;
}

int main(void)
{   
    int i = 0;
    pthread_t tid[2];

    for (i; i < 2; i++)
    {
        pthread_create(&tid[i], NULL, thread, NULL);
        if (i == 0)
            sleep(9);
    }

    while (1)
        sleep(100000);
    return 0;
}
