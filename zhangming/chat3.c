#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>

#define LOCALIPADDR "192.168.1.100"
#define REMOTIPADDR "192.168.1.112"
#define CHATPORT 8888

int main(void)
{
    int sd, ret, newsd;
    char buf[512];
    struct sockaddr_in myend, hisend;
    socklen_t len;
    struct pollfd fds[2];

    len = sizeof(myend);
    myend.sin_family = AF_INET;
    myend.sin_port = htons(CHATPORT);
    inet_pton(AF_INET, LOCALIPADDR, &myend.sin_addr);
    hisend.sin_family = AF_INET;
    hisend.sin_port = htons(CHATPORT);
    inet_pton(AF_INET, REMOTIPADDR, &hisend.sin_addr);


    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1)
    {
        perror("socket");
        exit(1);
    }

    ret = bind(sd, (struct sockaddr *)&myend, len);
    if (-1 == ret)
    {
        perror("bind");
        exit(1);
    }

    while (1)
    {
        printf("chat1 > chat2: ");
        fflush(stdout);

        if (fds[0].revents & POLLIN)
        {
            newsd = accept(sd, NULL, NULL);
            if (-1 == newsd)
            {
                perror("accept");
                exit(1);
            }

            ret = read (newsd, buf, 512);
            buf[ret] = '\0';
            printf("\nchat2 > chat1: %s\n", buf);
        }

        if (fds[1].revents & POLLIN)
        {
            ret = connect(sd, (struct sockaddr *)&hisend, len);
            ret = read(sd, buf, 512);
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
                ret = sendto(sd, buf, strlen(buf), 0, 
                        (struct sockaddr *)&hisend, len);
                if (-1 == ret)
                {
                    perror("sendto");
                    exit(1);
                }
            }
        }
    }
}

