#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SOCKETPATH "/tmp/serverend"

int main(int c, char **v)
{
    int sd, ret;
    struct sockaddr_un serverend;
    socklen_t len;
    char buf[512];

    len = sizeof(serverend);
    serverend.sun_family = AF_UNIX;
    strncpy(serverend.sun_path, SOCKETPATH, 108);

    sd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (-1 == sd)
    {
        perror("socket");
        exit(1);
    }

#if 0
    unlink(SOCKETPATH);
    ret = bind(sd, (struct sockaddr *)&serverend, len);
    if (-1 == ret)
    {
        perror("bind");
        exit(1);
    }
#endif
    sprintf(buf, "hello, I am %d!, %s", getpid(), v[1]);
    ret = sendto(sd, buf, strlen(buf), 0, (struct sockaddr *)&serverend, len);
    if (-1 == ret)
    {
        perror("sendto");
        exit(1);
    }

    close(sd);
    exit(0);
}
