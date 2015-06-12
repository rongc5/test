#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(int c, char **v)
{
    if (c != 3)
    {
        fprintf(stderr, "Usage...\n");
        exit(1);
    }
    pid_t pid;
    sigval_t val;
    int ret;

    pid = atoi(v[1]);
    val.sival_int = atoi(v[2]);

    ret = sigqueue(pid, SIGUSR1, val);
    if (-1 == ret)
    {
        perror("sigqueue");
        exit(1);
    }

    exit(0);
}
