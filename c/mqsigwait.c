#include <mqueue.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

int main(int c, char **v)
{
    int signo;
    mqd_t mqd;
    void * buff;
    ssize_t n;
    sigset_t newmask;
    struct mq_attr attr;
    struct sigevent sigev;

    if (c != 2) {
        perror("usage: mqsend!\n");
        return 1;
    }
    
    mqd = mq_open(v[1], O_RDONLY | O_NONBLOCK);
    mq_getattr(mqd, &attr);
    buff = malloc(attr.mq_msgsize);

    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &newmask, NULL);

    sigev.sigev_notify = SIGEV_SIGNAL;
    sigev.sigev_signo = SIGUSR1;
    mq_notify(mqd, &sigev);

    for (;;) {
        sigwait(&newmask, &signo);
        if (signo == SIGUSR1) {
           mq_notify(mqd, &sigev);
           while ((n = mq_receive(mqd, buff, attr.mq_msgsize, NULL)) >= 0) {
                printf("read %ld bytes\n", (long)n);
           }
           if (errno != EAGAIN)
               printf("空\n");
        }
    }

    free(buff);
    exit(0);
}

