#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SOCKETPATH "/tmp/serverend"

int main(void)
{
    int sd, ret;
    struct sockaddr_un serverend;
    socklen_t len;
    char buf[512];

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

    unlink(SOCKETPATH);
    ret = bind(sd, (struct sockaddr *)&serverend, len);
    if (-1 == ret)
    {
        perror("bind");
        exit(1);
    }

    while (1)
    {
        ret = recvfrom(sd, buf, 512, 0, NULL, NULL);
        if (ret == -1)
        {
            perror("recvfrom");
            exit(1);
        }
        buf[ret] = '\0';
        printf("recv: %s\n", buf);
    }
}
