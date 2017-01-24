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
#include "../src/common_def.h"
#include "../src/base_def.h"

#define SERVERADDR "127.0.0.1"
#define SERVERPORT 8

int main(int c, char **v)
{
    if (c != 3)
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
    addr.sin_port = htons(atoi(v[2]));
    inet_pton(AF_INET, v[1], &addr.sin_addr);

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

    _pass_msg_t head;
    
    printf("connect success!\n");
    head.body_len = sizeof("hello world");
    memcpy(buf, &head, sizeof(head));
    ret = sprintf(buf + sizeof(head), "%s", "hello world");

    while (1) {
        write(sd, buf, strlen(buf));
        sleep(3);
    }

    while (1)
        sleep(1);

    close(sd);
    exit(0);
}

