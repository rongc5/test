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

#define SERVERADDR "61.4.185.35"
#define SERVERPORT 80

void parse(char *buf)
{
    char * pos;
    int i = 0;
    if (pos = strstr(buf, "city"))
    {
        while (pos[7 + i++] != '\"');
        pos[7 + i - 1] = '\0';
        printf("城市:%s\n", pos + 7);
        pos[7 + i - 1] = '\"';
    }
    if (pos = strstr(buf, "temp"))
    {
        i = 0;
        while (pos[7 + i++] != '\"');
        pos[7 + i - 1] = '\0';
        printf("温度:%s\n", pos + 7);
        pos[7 + i - 1] = '\"';
    }
    if (pos = strstr(buf, "WD"))
    {
        i = 0;
        while (pos[7 + i++] != '\"');
        pos[5 + i - 1] = '\0';
        printf("风向:%s\n", pos + 5);
        pos[5 + i - 1] = '\"';
    }
}

int main(int argc, char **argv)
{
    int sd, fdm, ret;
    struct sockaddr_in addr;
    socklen_t len;
    char buf[4096];
    struct pollfd fds;

    len = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVERPORT);
    inet_pton(AF_INET, SERVERADDR, &addr.sin_addr);

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
    sprintf(buf, "%s%s\r\n", "GET /data/sk/101010100.html\r\n",
            "Host: www.weather.com.cn\r\n");
    write(sd, buf, strlen(buf));

    fds.fd = sd;
    fds.events = POLLIN;

    while (1)
    {
        ret = poll(&fds, 1, 3000);
        if (0 == ret)
        {
            printf("transport end!\n");
            break;
        }

        ret = read(sd, buf, 512);
        if (0 == ret)
            break;
        buf[ret] = '\0';
        parse(buf);
        break;
    }

    close(sd);
    exit(0);
}
