#include <mqueue.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int c, char **v)
{
    int b, flags;
    mqd_t mqd;

    flags = O_RDWR | O_CREAT;
    while ((b = getopt(c, v, "e")) != -1) {
        switch (b) {
            case 'e':
                flags | O_EXCL;
                break;
        }
    }

    if (optind != c -1)
        printf("error , and stop!\n");

    mqd = mq_open(v[optind], flags, 0755, NULL);

    mq_close(mqd);

    exit(0);
}

