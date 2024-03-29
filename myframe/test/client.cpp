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
#include "common_def.h"

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
    char buf[4096], tmp[4096];
    socklen_t len;
    int sd, ret;

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

    CommonMsg msg;;
    msg.set_obj_id(1);
    msg.set_obj_op(1);
    msg.set_version(1);
    msg.set_reserved(0);

    sprintf(buf, "%s", "hello world");
    msg.set_str(buf);
    
    string out;
    msg.SerializeToString(&out);

    int length = out.size();
    memset(buf, 0, sizeof(buf));

    memcpy(buf, &length, sizeof(length));
    memcpy(buf+sizeof(length), out.c_str(), out.size());

    printf("%d %d\n", out.size(), strlen(buf));
    //write(sd, "hello", sizeof("hello world"));
    while (1) {
        write(sd, buf, out.size() + sizeof(length));
        //write(sd, "hello", sizeof("hello world"));
        read(sd, tmp, sizeof(tmp));
        printf("recv: %s\n", tmp);
        //printf("recv: \n");
        sleep(3);
    }

    close(sd);
    exit(0);
}

