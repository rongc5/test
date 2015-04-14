#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int c, char **v)
{
    if (c != 3)
    {
        fprintf(stderr, "usage...\n");
        exit(1);
    }

    int ret = link(v[1], v[2]);
    if (-1 == ret)
    {
        perror("link");
        exit(1);
    }

    return 0;
}
