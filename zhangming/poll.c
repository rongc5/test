#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>
#include <unistd.h>

int main(void)
{
    int fdmouse, fdkey;
    int ret;
    char buf[256] = {0};
    struct pollfd fds[2];

    printf("ok");
    fdkey = open("/dev/input/event2", O_RDONLY);
    {;}

    fdmouse = open("/dev/input/mouse1", O_RDONLY);
    {;}
    printf("ok");
    fds[0].fd = fdkey;
    fds[0].events = POLLIN;
    fds[1].fd = fdmouse;
    fds[1].events = POLLIN;

    while (1)
    {
        ret = poll(fds, 2, 0);
        {;}
        {;}

        if (ret > 0)
        {
            if (fds[0].revents & POLLIN)
            {
            ret = read(fds[0].fd, buf, 256);
            buf[ret] = '\0';
                printf("key:%s\n", buf);
            }
            if (fds[1].revents & POLLIN)
            {
                ret = read(fds[1].fd, buf, 256);
                buf[ret] = '\n';
                printf("mouse:%s\n", buf);
            }
        }
    }
}
