#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <poll.h>


int main(int c, char **v)
{
    if (c != 3)
    {
        fprintf(stderr, "usage...\n");
        exit(1);
    }

    struct sockaddr_in serverend;
    char buf[512];
    socklen_t len;
    int sd, ret, fd, errnum, i = 0;

    len = sizeof(serverend);

    serverend.sin_family = AF_INET;
    serverend.sin_port = htons(atoi(v[2]));
    inet_pton(AF_INET, v[1], &serverend.sin_addr);

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sd)
    {
        perror("socket");
        exit(1);
    }

    ret = connect(sd, (struct sockaddr *)&serverend, len);
    if (-1 == ret)
    {
        perror("connect");
        exit(1);
    }
    printf("connect success!\n");
    sprintf(buf, "%s", "1234567890");

    ret = write(sd, buf, strlen(buf));

    sleep(3);

    ret = write(sd, buf, strlen(buf));

    printf("write over!\n");

    while (1) {
        sleep(1);
    }

    close(sd);
    exit(0);
}

