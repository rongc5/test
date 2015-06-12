#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
    pid_t pid[5];
    int i = 0;
    int fd[5][2];
    int ret;
    char buf[10];
    int k = 5;
    srand(getpid());

    nfds = fd[0];
    for (i; i < 5; i++)
    {
        if (fd[i] > nfds)
            nfds = fd[i];
    }
    nfds++;
    for (i; i < 5; i++)
    {
        FD_ZERO(fds)
        FD_SET(fd(0), &fds);
        FD_SET(fd(1), &fds);
        FD_SET(fd(2), &fds);
        FD_SET(fd(3), &fds);
        FD_SET(fd(4), &fds);
        ret = select(nfds, &fds, NULL, NULL, NULL);
        
        if (ret > 0)
        {
            if (FD_ISSET(fd[0], ))

        }

    }

    exit(0);
}
