#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int c, char **v)
{
    if (c != 2)
    {
        fprintf(stderr, "usage...\n");
        exit(1);
    }

    int ret = mkdir(v[1], 0777);
    if (-1 == ret)
    {
        perror("mkdir");
        return -1;
    }

    return 0;
}
