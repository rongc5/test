#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>

#define LOCALIPADDR "192.168.1.103"
#define REMOTIPADDR "192.168.1.103"
#define CHATPORT 8888

int main(void)
{
    int sd, ret;
    char buf[512];
    struct sockaddr_in myend, hisend;
    struct pollfd fds[2];
    socklen_t len;

    len = sizeof(myend);
    myend.sin_family = AF_INET;
    myend.sin_port = htons(CHATPORT);
    inet_pton(AF_INET, LOCALIPADDR, &myend.sin_addr);
    hisend.sin_family = AF_INET;
    hisend.sin_port = htons(CHATPORT);
    inet_pton(AF_INET, REMOTIPADDR, &hisend.sin_addr);

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd == -1) {
        perror("socket");
        exit(1);
    }

//    ret = bind(sd, (struct sockaddr *)&myend, len);
    if (ret == -1) {
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
        if (ret == -1) 
        {
            perror("poll");
            exit(1);
        }

        if (fds[0].revents & POLLIN)
        {
            ret = recvfrom(sd, buf, 512, 0, NULL, NULL);
            if (ret == -1) 
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
            if (ret == 1)
                continue;
            else if (ret > 1)
            {
                buf[ret-1] = '\0';
                if (strcmp(buf, "exit") == 0)
                {
                    close(sd);
                    exit(0);
                }
                ret = sendto(sd, buf, strlen(buf), 0, (struct sockaddr *)&hisend, len);
                if (ret == -1)
                {
                    perror("sendto");
                    exit(1);
                }
            }
        }
    }
}

