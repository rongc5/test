#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>

#define SOCKETPATH "/tmp/serverend"

int main(void)
{
    int sd, ret;
    char buf[512];
    struct sockaddr_un serverend;
    socklen_t len;

    len = sizeof(serverend);
    serverend.sun_family = AF_UNIX;
    strncpy(serverend.sun_path, SOCKETPATH, 108);

    daemon(1, 1);
    sd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sd == -1)
    {
        perror("socket");
        exit(1);
    }
#if 1
    unlink(SOCKETPATH);
    ret = bind(sd, (struct sockaddr *)&serverend, len);
    if (-1 == ret)
    {
        perror("bind");
        exit(1);
    }
#endif
    while (1)
    {
        ret = recvfrom(sd, buf, 512, 0, NULL, NULL);
        if (-1 == ret)
        {
            perror("recvfrom");
            exit(1);
        }
        buf[ret] = '\0';
        printf("recv: %s", buf);
    }

}

