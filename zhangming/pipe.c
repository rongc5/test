#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int main(void)
{
    int fd[2];
    int ret;
    pid_t pid;
    char buf[256];

    ret = pipe(fd);
    if (-1 == ret)
    {
        perror("pipe");
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
        close(fd[1]);
        while (1)
        {
            ret = read(fd[0], buf, 256);
            if (0 == ret)
                break;
            write(1, buf, ret);
        }
        printf("child exit!, mypid is %d, myfather is %d\n",
                getpid(), getppid());
        close(fd[0]);
        exit(0);
    }

    close(fd[0]);
    sprintf(buf, "my pid is %d\n", getpid());
    write(fd[1], buf, strlen(buf));

//    wait(NULL);
    printf("exit\n");
    close(fd[1]);
    exit(0);
}
