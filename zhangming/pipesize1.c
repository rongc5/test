#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int cnt = 0;

void sig_handler(int s)
{
    printf("pipe size = %d\n", cnt);
    exit(0);
}

int main(void)
{
    int fd[2], ret;
    struct sigaction act;
    act.sa_handler = sig_handler;
    act.sa_flags = SA_RESTART;
    sigemptyset(&act.sa_mask);

    sigaction(SIGALRM, &act, NULL);

    ret = pipe(fd);
    if (-1 == ret)
    {
        perror("pipe");
        exit(1);
    }

    while (1)
    {
        ret = write(fd[1], "a", 1);
        cnt += ret;
        alarm(1);
    }

    return 0;
}
