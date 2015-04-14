#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <errno.h>
#include <poll.h>

#define SERVERADDR "210.73.64.80"
#define SERVERPORT 80

int main(int c, char **v)
{
    if (c != 2)
    {
        fprintf(stderr, "usage...\n");
        exit(1);
    }

    struct sockaddr_in addr;
    char buf[4096];
    socklen_t len;
    int sd, ret, fd;
    struct pollfd fds;

    len = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVERPORT);
    inet_pton(AF_INET, SERVERADDR, &addr.sin_addr);

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sd)
    {
        perror("socket");
        exit(1);
    }

    ret = connect(sd, (struct sockaddr *)&addr, len);
    if (-1 == ret)
    {
        perror("connect");
        exit(1);
    }
    printf("connect success!\n");
    sprintf(buf, "%s\r\n", "GET /data/cloud/1107131502.JPG\r\n");
    write(sd, buf, strlen(buf));

    fd = creat(v[1], 0666);
    if (-1 == fd)
    {
        perror("creat");
        exit(1);
    }

    fds.fd = sd;
    fds.events = POLLIN;

    while (1)
    {
#if 0
        ret = poll(&fds, 1, 3000);
        if (0 == ret)
        {
            printf("transport end!\n");
            break;
        }
#endif
        ret = read(sd, buf, 512);
        if (0 == ret)
        {
            printf("read end!\n");
            break;
        }
        write(fd, buf, ret);
        write(1, ".....", 5);
    }
    close(fd);
    close(sd);
    exit(0);
}

