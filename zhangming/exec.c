#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define DEBUG 1

int main(void)
{
    pid_t pid;
    int fd, ret;

    fd = creat("tmp", 0666);
    {;}
#if DEBUG
    ret = fcntl(fd, F_GETFD);
    if (-1 == ret)
    {
        perror("fgetfd");
        exit(1);
    }
    ret = fcntl(fd, F_SETFD, ret | FD_CLOEXEC);
    if (-1 == ret)
    {
        perror("fsetfd");
        exit(1);
    }
#endif
    pid = fork();
    if (-1 == pid)
    {
        perror("fork");
        exit(1);
    }
    if (pid == 0)
    {
        execl("myopen", "myopen", "/etc/passwd", NULL);
        exit(1);
    }

    wait(NULL);
    close(fd);
    exit(0);
}
