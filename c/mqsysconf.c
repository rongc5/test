#include <mqueue.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int c, char **v)
{
    printf("MQ_OPEN_MAX = %ld, MQ_PRIO_MAX = %ld\n", \
            sysconf(_SC_MQ_OPEN_MAX), sysconf(_SC_MQ_PRIO_MAX));

    exit(0);
}

