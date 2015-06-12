#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int daemonsize(int nochdir, int noclose)
{
    int fd;
    pid_t pid;

    pid = fork();
    if (-1 == pid)
    {
        perror("fork");
        return -1;
    }
    if (pid > 0)
        exit(0);

    umask(0);
    setsid();
    if (nochdir == 0)
        chdir("/");
    if (noclose == 0)
    {
        fd = open("/dev/null", O_RDONLY);
        if (-1 == fd)
        {
            perror("open");
            return -1;
        }
        dup2(fd, 0);
        close(fd);
        fd = open("/dev/null", O_WRONLY);
        if (-1 == fd)
        {
            perror("open");
            return -1;
        }
        dup2(fd, 1);
        close(fd);
        fd = open("/dev/null", O_WRONLY);
        if (-1 == fd)
        {
            perror("open");
            return -1;
        }
        dup2(fd, 2);
        close(fd);
    }

    return 0;
}

int main(void)
{
    daemonsize(1, 1);

    while (1)
    {
        printf("daemon!\n");
        sleep(5);
    }
}
