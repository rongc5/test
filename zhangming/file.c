#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
    pid_t pid;
    int fd, ret;
    char buf[32] = {0};

    fd = open("/etc/passwd", O_RDONLY);
    if (-1 == fd)
    {
        perror("open");
        exit(1);
    }

    pid = fork();
    if (-1 == pid)
    {
        perror("fork");
        exit(1);
    }

    if (0 == pid)
    {
        ret = read(fd, buf, 10);
        if (-1 == ret)
        {
            perror("read");
            exit(1);
        }

        printf("child:%s\n", buf);
        close(fd);
        exit(0);
    }

    wait(NULL);

    ret = read(fd, buf, 10);
    if (-1 == ret)
    {
        perror("read");
        exit(1);
    }

    printf("father:%s\n", buf);
    close(fd);
    exit(0);
}
