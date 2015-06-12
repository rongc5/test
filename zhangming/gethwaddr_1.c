#include <stdio.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/ethernet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>

int main(int c, char **v)
{
    if (c != 2)
    {
        fprintf(stderr, "usage...\n");
        exit(1);
    }

    int sd, ret;
    struct ifreq req;
    char ipbuf[16];

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == sd)
    {
        perror("socket");
        exit(1);
    }

    strcpy(req.ifr_name, v[1]);

    ret = ioctl(sd, SIOCGIFCONF , &req);
    if (-1 == ret)
    {
        perror("ioctl");
        exit(1);
    }

    printf("%s\n", (char *)inet_ntop(AF_INET, &req.ifr_addr, ipbuf, 16));
}
