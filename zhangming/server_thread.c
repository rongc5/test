#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <string.h>
#include <pthread.h>

#define SERVERPATH "/tmp/serverend"

void * thread_handler(void *arg)
{
    int ret;
    char buf[512];

    int newsd = (int)arg;
    ret = read(newsd, buf, 512);
    if (-1 == ret)
    {
        perror("read");
        return NULL;
    }
    if (ret == 0)
    {
        fprintf(stderr, "end of file!\n");
        return NULL;
    }else if (ret > 0)
    {
        buf[ret] = '\0';
        printf("from client: %s\n", buf);
        write(newsd, "hello qianfeng", 23);
    }
    sleep(3);
    close(newsd);
    return NULL;
}

int main(void)
{
    int sd, ret, newsd;
    struct sockaddr_un serverend;
    socklen_t len;
    char buf[512];
    pthread_t tid;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    len = sizeof(serverend);
    serverend.sun_family = AF_UNIX;
    strcpy(serverend.sun_path, SERVERPATH);

    daemon(1, 1);
    sd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (-1 == sd)
    {
        perror("socket");
        exit(1);
    }

    unlink(SERVERPATH);
    ret = bind(sd, (struct sockaddr *)&serverend, len);
    if (ret = -1)
    {
        perror("bind");
        exit(1);
    }

    listen(sd, 10);

    while (1)
    {
        newsd = accept(sd, NULL, NULL);
        ret = pthread_create(&tid, &attr, thread_handler, (void *)newsd);
        if (ret)
        {
            fprintf(stderr, "pthread_create:%s\n", strerror(ret));
            exit(1);
        }
    }
}
