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

    sd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sd == -1) {
        perror("socket");
        exit(1);
    }

#if 0
    ret = bind(sd, (struct sockaddr *)&serverend, len);
    if (ret == -1) {
        perror("bind");
        exit(1);
    }
#endif
    sprintf(buf, "hello, I am %d!", getpid());
    ret = sendto(sd, buf, strlen(buf), 0, 
            (struct sockaddr *)&serverend, len);
    if (ret == -1) {
        perror("sendto");
        exit(1);
    }

    close(sd);
    exit(0);
}

