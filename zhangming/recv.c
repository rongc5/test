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

    fd = open("dop", O_RDONLY);
    {}
    printf("open sucess!\n");

    ret = read(fd, buf, 256);
    {}

    write(1, buf, ret);

    close(fd);

    exit(0);
}
