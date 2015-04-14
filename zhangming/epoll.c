#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>
#include <sys/epoll.h>
#include <unistd.h>

int main(void)
{
    int fdmouse, fdkey;
    int ret;
    char buf[256];
    struct epoll_event fds[2], revents;
    int epfd;

    epfd = epoll_create(1);
    if (epfd == -1)
    {;}

    fdkey = open("/dev/input/event1", O_RDONLY);
    {;}

    fdmouse = open("/dev/input/mouse1", O_RDONLY);
    {;}

    fds[0].data.fd = fdkey;
    fds[0].events = EPOLLIN;
    fds[1].data.fd = fdmouse;
    fds[1].events = EPOLLIN;

    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fdkey, &fds[0]);
    {;}

    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fdmouse, &fds[1]);
    {;}

     while (1)
     {
         ret = epoll_wait(epfd, &revents, 1, -1);
         {;}
         {;}

         if (ret > 0)
         {
             if (revents.events & EPOLLIN)
             {
                 ret = read(revents.data.fd, buf, 256);
                 buf[ret] = '\0';
                 if (revents.data.fd == fdkey)
                     printf("key:%s\n", buf);
                 else
                     printf("mouse:%s\n", buf);
             }
         }
     }
}
