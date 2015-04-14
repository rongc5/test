#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
    int fd;
    char buf[256];
    int ret;

    fd = open("dot", O_RDONLY);
    if (-1 == fd)
    {
        perror("open");
        exit(1);
    }
    printf("open sucess!\n");

    ret = read(fd, buf, 256);
    if (-1 == ret)
    {
        perror("read");
        exit(1);
    }else if (0 == ret)
    {
        printf("read end!\n");
        exit(1);
    }

    write(1, buf, ret);

    close(fd);
    exit(0);
}
