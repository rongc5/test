#include <stdio.h>
#include <stdlib.h>

int main(int c, char **v)
{
    if (c != 3)
    {
        fprintf(stderr, "usage...\n");
        return -1;
    }

    int ret = rename(v[1], v[2]);
    if (-1 == ret)
    {
        perror("rename");
        exit(1);
    }

    return 0;
}
