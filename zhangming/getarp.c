#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>

int main(int c, char **v)
{
    if (c != 2)
    {
        fprintf(stderr, "usage...\n");
        exit(1);
    }

    int ret, sd;
    struct arpreq req;
    struct sockaddr_in *addr;
    struct sockaddr_in dst;

    memset(&req, 0, sizeof(req));

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == sd)
    {
        perror("socket");
        exit(1);
    }

    dst.sin_family = AF_INET;
    inet_pton(AF_INET, v[1], &dst.sin_addr);

    memcpy(&req.arp_pa, &dst, sizeof(dst));
    strcpy(req.arp_dev, "eth1");

    ret = ioctl(sd, SIOCGARP, &req);
    if (-1 == ret)
    {
        perror("ioctl");
        exit(1);
    }

    printf("mac:%s\n", (char *)ether_ntoa(req.arp_ha.sa_data));
    close(sd);
    exit(1);
}
