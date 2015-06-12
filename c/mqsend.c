#include <mqueue.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int c, char **v)
{
    mqd_t mqd;
    void * ptr;
    size_t len;
    unsigned int prio;

    if (c != 5) {
        perror("usage: mqsend!\n");
        return 1;
    }
    
    len = atoi(v[2]);
    prio = atoi(v[3]);

    mqd = mq_open(v[1], O_WRONLY);

    ptr = calloc(len, sizeof(char));

    memcpy(ptr, v[4], len);
    mq_send(mqd, ptr, len, prio);

    free(ptr);
    exit(0);
}

