#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sig_handler(int s)
{
    printf("sigpipe was catched!\n");
}

int main(void)
{
    int fd[2], ret;
    struct sigaction act;

    act.sa_handler = sig_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESTART;

    sigaction(SIGPIPE, &act, NULL);

    ret = pipe(fd);
    if (-1 == ret)
    {
        perror("pipe");
        exit(1);
    }

    close(fd[0]);
    printf("write!\n");
    write(fd[1], "a", 1);

    printf("exit!\n");
    close(fd[1]);
    return 0;
}
