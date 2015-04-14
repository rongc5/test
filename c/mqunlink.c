#include <mqueue.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int c, char **v)
{
    if (c != 2) {
        perror("stop!\n");
        return 1;
    }

    mq_unlink(v[1]);

    exit(0);
}

