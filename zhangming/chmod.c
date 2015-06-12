#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int c, char **v)
{
    if (c != 3)
    {
        fprintf(stderr, "usage....\n");
        exit(1);
    }

    int ret = chmod(v[1], strtol(v[2], (char **)NULL, 8));
    if (ret == -1)
    {
        perror("chmod");
        exit(1);
    }

    return 0;
}
