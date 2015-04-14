#include <mqueue.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int c, char **v)
{
    int b, flags;
    mqd_t mqd;
    void * buff;
    size_t n;
    unsigned int prio;
    struct mq_attr attr;

    flags = O_RDONLY;
    
    while (b = getopt(c, v, "n") != -1) {
        switch (c) {
            case 'n':
                flags |= O_NONBLOCK;
                break;
        }
    }

    if (optind != c - 1) {
        printf("stop!\n");
        return 1;
    }


    mqd = mq_open(v[optind], flags);
    mq_getattr(mqd, &attr);

    buff = malloc(attr.mq_msgsize);

    mq_receive(mqd, buff, attr.mq_msgsize, &prio);
    printf("read %ld bytes, prioty = %u, data: %s\n", (long)n, prio, buff);
    free(buff);

    exit(0);
}

