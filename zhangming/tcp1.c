#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#define SERVERADDR "192.168.1.103"
#define SERVERPORT 8888

int main(void)
{
    int sd, ret;
    char buf[6553];
    struct sockaddr_in serverend;
    socklen_t len;
    int newsd;

    len = sizeof(serverend);
    serverend.sin_family = AF_INET;
    serverend.sin_port = htons(SERVERPORT);
    inet_pton(AF_INET, SERVERADDR, &serverend.sin_addr);

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1) {
        perror("socket");
        exit(1);
    }

    ret = bind(sd, (struct sockaddr *)&serverend, len);
    if (ret == -1) {
        perror("bind");
        exit(1);
    }

    listen(sd, 10);
    newsd = accept(sd, NULL, NULL);
    printf("new conncet sd=%d\n", newsd);

    close(newsd);
    close(sd);
    exit(0);
}
