#include <stdio.h>
#include <unistd.h>

int main(int c, char **v)
{
    if (c != 2)
    {
        fprintf(stderr, "usage...\n");
        return -1;
    }

    int ret = rmdir(v[1]);
    if (-1 == ret)
    {
        perror("rmdir");
        return -1;
    }

    return 0;
}
