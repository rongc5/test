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

    fd = open("dop", O_WRONLY);
    {}
    fprintf(stdout,"open success!\n");
    sleep(10);
    sprintf(buf, "pid %d send you a message!", getpid());
//    exit(0);
    ret = write(fd, buf, strlen(buf));
    {}

    close(fd);
    exit(0);
}

