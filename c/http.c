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

#define SERVERADDR "115.182.15.71"
#define SERVERPORT 80

int main(int argc, char **argv)
{
    int sd, fdm, ret;
    struct sockaddr_in addr;
    socklen_t len;
    char buf[4096];
    char msg[512];
    struct pollfd fds;

    len = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVERPORT);
    inet_pton(AF_INET, SERVERADDR, &addr.sin_addr);
#if 1
    sd = socket(AF_INET, SOCK_STREAM, 0);
#else 
    sd = socket(AF_INET, SOCK_DGRAM, 0);
#endif
    if (-1 == sd)
    {
        perror("socket");
        exit(1);
    }
#if 1
    ret = connect(sd, (struct sockaddr *)&addr, len);
    if (-1 == ret)
    {
        perror("connect");
        exit(1);
    }
#endif 
    fds.fd = 0;
    fds.events = POLLIN;

    while (1)
    {
        ret = poll(&fds, 1, 3000);
        if (fds.revents & POLLIN)
        {
            
            ret = read(0, msg, 512);
            if (ret <= 1)
                continue;
            msg[ret - 1] = '\0';
            sprintf(buf, "%s%s%s\r\n", "GET /fetion.php?m=13466349692&p=scat952&t=13466349692&msg=", msg,
            "   HTTP/1.1\r\n", "Host: feixinapi.sinaapp.com\r\n");
            printf("buf:%s\n", buf);
            write(sd, buf, strlen(buf));
        }
    }

    close(sd);
    exit(0);
}
