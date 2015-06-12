#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <string.h>

#define SERVERPATH "/tmp/serverend"

int main(void)
{
    int sd, ret, newsd;
    struct sockaddr_un serverend;
    socklen_t len;
    char buf[512];

    len = sizeof(serverend);
    serverend.sun_family = AF_UNIX;
    strcpy(serverend.sun_path, SERVERPATH);

    sd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (-1 == sd)
    {
        perror("socket");
        exit(1);
    }

    ret = connect(sd, (struct sockaddr *)&serverend, len);
    if (ret == -1) {
        perror("connect");
        exit(1);
    }

    printf("connect success!\n");
    write(sd, "hello world", 11);

    ret = read(sd, buf, 512);
    buf[ret] =  '\0';
    printf("From server:%s\n", buf);

    sleep(5);
    close(sd);
    exit(0);
}
