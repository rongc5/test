#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>          
#include <sys/socket.h>

int fd[2];

void *thread1(void *arg)
{
    while (1)
    {
        int i=0;
        write(fd[0], "hello world", sizeof("hello world"));
        sleep(1);
    }
}

void *thread2(void *arg)
{
    while (1)
    {
        int i=0;
        char buf[512] = {0};
        read(fd[1], buf, sizeof(buf));
        printf("buf [%s] %p\n", buf, &i);
        sleep(1);
    }
}



int main()
{

    int ret = socketpair(AF_UNIX,SOCK_STREAM,0,fd);

    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, thread1, NULL);
    pthread_create(&tid2, NULL, thread2, NULL);

    while (1)
        sleep(99);

    return 0;
}
