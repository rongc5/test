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

    fd = open("dot", O_WRONLY);
    if (-1 == fd)
    {
        perror("open");
        exit(1);
    }
    printf("open sucess!\n");
    sleep(10);
    sprintf(buf, "pid %d send you a message!", getpid());
    ret = write(fd, buf, strlen(buf));
    if (-1 == ret)
    {
        perror("write");
        exit(1);
    }

    close(fd);
    exit(0);
}
