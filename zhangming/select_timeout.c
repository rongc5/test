#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>

int main(void)
{
    struct timeval tm;
    tm.tv_sec = 5;
    tm.tv_usec = 0;

    fd_set fds;
    int nfds;
    int fd = open("lg", O_RDONLY);
    nfds = fd + 1;

    int ret;

    while (1)
    {
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        ret = select(nfds, NULL, &fds, NULL, &tm);
        if (ret == -1)
        {
            perror("select");
            exit(1);
        }
            printf("%.2f\n", (float)tm.tv_sec);
    }
}
