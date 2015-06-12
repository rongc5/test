#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>

#define SERVERADDR "192.168.1.104"
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

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == sd)
    {
        perror("socket");
        exit(0);
    }

    ret = bind(sd, (struct sockaddr *)&serverend, len);
    if (-1 == ret)
    {
        perror("bind");
        exit(1);
    }

    listen(sd, 10);
    newsd = accept(sd, NULL, NULL);
    printf("new connect sd = %d\n", newsd);
    
    close(newsd);
    close(sd);
    exit(0);
}
