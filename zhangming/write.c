#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
    int fd;
    int ret;

    fd = open("tmp", O_RDWR);
    if (-1 == fd)
    {
        perror("open");
        exit(1);
    }

    write(fd, "hello", 5);

    close(fd);

    exit(0);
}
