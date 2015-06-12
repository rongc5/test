#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

void * thread_handler(void *arg)
{
    char c;
    printf("c aar = %p(%c, %d)\n", &c, c, c);
}

int main(void)
{
    int ret;
    pthread_t tid;
    pthread_attr_t attr;
    char *stackaddr;
    size_t stacksize;

    stacksize = 16384;
    stackaddr = (char *)mmap(NULL, stacksize, PROT_READ | PROT_WRITE,
            MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (stackaddr == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }
    memset(stackaddr, 'a', stacksize);
    printf("mmap:addr = %p\n", stackaddr);
    pthread_attr_init(&attr);

    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    ret = pthread_attr_setstack(&attr, stackaddr, stacksize);
    if (ret)
    {
        fprintf(stderr, "setstack:%s\n", strerror(ret));
        exit(1);
    }

    ret = pthread_create(&tid, &attr, thread_handler, NULL);
    if (ret)
    {
        fprintf(stderr, "create:%s\n", strerror(ret));
        exit(1);
    }

    pthread_exit(NULL);
}
