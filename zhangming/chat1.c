#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>

#define CHAT1PATH "/tmp/chat1"
#define CHAT2PATH "/tmp/chat2"

int main(void)
{
    int sd, ret;
    char buf[512];
    struct sockaddr_un myend, hisend;
    struct pollfd fds[2];
    socklen_t len;

    len = sizeof(myend);
    myend.sun_family = AF_UNIX;
    strncpy(myend.sun_path, CHAT1PATH, 108);
    hisend.sun_family = AF_UNIX;
    strncpy(hisend.sun_path, CHAT2PATH, 108);

    sd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sd == -1)
    {
        perror("socket");
        exit(1);
    }

    unlink(CHAT1PATH);
    ret = bind(sd, (struct sockaddr *)&myend, len);
    if (-1 == ret)
    {
        perror("bind");
        exit(1);
    }

    fds[0].fd = sd;
    fds[0].events = POLLIN;
    fds[1].fd = 0;
    fds[1].events = POLLIN;

    while (1)
    {
        printf("chat1 > chat2: ");
        fflush(stdout);
        ret = poll(fds, 2, -1);
        if (-1 == ret)
        {
            perror("poll");
            exit(1);
        }

        if (fds[0].revents & POLLIN)
        {
            ret = recvfrom(sd, buf, 512, 0, NULL, NULL);
            if (-1 == ret)
            {
                perror("recvfrom");
                exit(1);
            }

            buf[ret] = '\0';
            printf("\nchat2 > chat1: %s\n", buf);
        }

        if (fds[1].revents & POLLIN)
        {
            ret = read(0, buf, 512);
            if (1 == ret)
                continue;
            else if (ret > 1)
            {
                buf[ret - 1] = '\0';
                if (strcmp(buf, "exit") == 0)
                {
                    close(sd);
                    exit(0);
                }
                ret = sendto(sd, buf, strlen(buf), 0, (struct sockaddr *
                            )&hisend, len);
                if (-1 == ret)
                {
                    perror("sendto");
                    exit(1);
                }
            }
        }
    }
}

