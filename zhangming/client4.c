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
    
    sd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sd == -1)
    {
        perror("socket");
        exit(1);
    }
#if 0
    ret = bind(sd, (struct sockaddr *)&serverend, len);
    if (-1 == ret)
    {
        perror("bind");
        exit(1);
    }
#endif

    sprintf(buf, "hello, I am %d!", getpid());
    ret = sendto(sd, buf, strlen(buf), 0, (struct sockaddr *)&serverend, len);
    if (-1 == ret)
    {
        perror("sendto");
        exit(1);
    }

    close(sd);
    exit(0);
}

