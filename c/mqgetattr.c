#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <unistd.h>

int main(int c, char **v)
{
    mqd_t mqd;
    struct mq_attr attr;

    if (c != 2)
        printf("usage: mggetattr\n");

    mqd = mq_open(v[1], O_RDONLY);

    mq_getattr(mqd, &attr);
    printf("max #msgs = %ld, max #bytes/msg = %ld, "\
            "currently on queue = %ld\n", \
            attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);

    mq_close(mqd);
    exit(0);
}
