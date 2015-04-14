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

#define SERVERADDR "61.217.250.15"
#define SERVERPORT 80

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "usage...\n");
        exit(1);
    }

    int sd, fd, ret;
    struct sockaddr_in addr;
    socklen_t len;
    char buf[4096];
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
    sprintf(buf, "%s%s\r\n", "GET /youku/557297F086C4A83FBDCEBE36C1/03000202004E1D4E4E8E0900C9A1D125C72079-49D1-C2AD-DD50-D977977187BC.flv HTTP/1.1\r\n", "Host: 60.217.250.15\r\n");
    write(sd, buf, strlen(buf));

    fd = creat(argv[1], 0666);
    if (-1 == fd)
    {
        perror("creat");
        exit(1);
    }

    fds.fd = sd;
    fds.events = POLLIN;

    while (1)
    {
        ret = poll(&fds, 1, 3000);
        if (0 == ret)
        {
            printf("transport end!\n");
            break;
        }

        ret = read(sd, buf, 512);
        if (0 == ret)
            break;
        write(fd, buf, 512);
        write(1, "...", 3);
    }

    close(sd);
    exit(0);
}
