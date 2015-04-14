#include <stdio.h>
#include <poll.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
    struct pollfd fds[1];
    int fd;
    pid_t pid;
    int ret;
    char buf[100];

    fd = open("entry", O_RDWR);
    if (-1 == fd) {
        perror("open");
        exit(1);
    }

    fds[0].fd = fd;
    fds[0].events = POLLIN;
    pid = fork();
    if (-1 == pid) {
        perror("fork");
        exit(1);
    }

    while (1) {
        ret = poll(fds, 1, 0);
        if (ret == 0)
            continue;
       
        if (pid == 0) {
            if (fds[0].revents & POLLIN)
            {
                ret = read(fds[0].fd, buf, 256);
                buf[ret] = '\0';
                printf("key:%s\n", buf);
            }
        } else {
                if (fds[0].revents & POLLIN) {
                ret = read(fds[0].fd, buf, 256);
                buf[ret] = '\0';
                printf("key:%s\n", buf);
            }       
        }
    }
}
