#include <stdio.h>
#include <unistd.h>

int main(int c, char **v)
{
    if (c != 3)
    {
        fprintf(stderr, "usage...\n");
        return -1;
    }

    int ret = truncate(v[1], atoi(v[2]));
    if (-1 == ret)
    {
        perror("truncate");
        return -1;
    }

    return 0;
}
