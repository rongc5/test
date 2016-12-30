#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <signal.h>
#include <poll.h>
#include <errno.h>

int main(int c, char **v)
{
    if (c != 3) {
        fprintf(stderr, "usage...\n");
        exit(1);
    }


    struct sockaddr_in serverend, clientend;
    char buf[512], ipbuf[16];
    socklen_t len;
    int sd, ret, fd, newsd;
    len = sizeof(serverend);
    pid_t pid;

    serverend.sin_family = AF_INET;
    serverend.sin_port = htons(atoi(v[2]));
    inet_pton(AF_INET, v[1], &serverend.sin_addr);

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sd)
    {
        perror("socket");
        exit(1);
    }

    ret = bind(sd, (struct sockaddr *)&serverend, len);
    if (-1 == ret)
    {
        perror("bind");
        exit(1);
    }

    listen(sd, 10);

    while (1)
    {
        newsd = accept(sd, (struct sockaddr *)&clientend, &len);
        if (-1 == newsd)
        {
            perror("accept");
            exit(1);
        }
        printf("Client addr:%s, port:%d connect success!\n",
                (char *)inet_ntop(AF_INET, &clientend.sin_addr, ipbuf, 16),  ntohs(clientend.sin_port));

        sleep(10);
        ret = read(newsd, buf, 512);
        if (-1 == ret)
        {
            perror("read");
            exit(1);
        }
        buf[ret] = '\0';
        printf("recv  %s\n", buf);

        close(newsd);
    }
    close(sd);
}

