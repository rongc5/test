#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int c, char **v)
{
    if (c != 2)
    {
        fprintf(stderr, "usage...\n");
        exit(1);
    }

    int fd;

    fd = open(v[1], O_RDONLY);
    if (-1 == fd)
    {
        perror("open");
        exit(1);
    }
    printf("%s, fd = %d\n", v[1], fd);

    close(fd);

    return 0;
}
