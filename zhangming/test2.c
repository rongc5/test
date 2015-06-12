#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
    int fd;
    int ret;
    char buf[6] = {0};

    fd = open("rong", O_RDONLY);
    if (-1 == fd)
    {
        perror("open");
        exit(1);
    }

    ret = read(fd, buf, 13);
    if (-1 == ret)
    {
        perror("read");
        exit(1);
    }
    printf("%s\n", buf);
    close(fd);

    exit(0);
}
