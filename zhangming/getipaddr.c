#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/ethernet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>

#define NETCARDNUM 10

int main(int c, char **v)
{
    if (c != 2) {
        fprintf(stderr, "usage...\n");
        exit(1);
    }

    int sd, ret;
    struct ifreq req[NETCARDNUM];
    struct ifconf conf;
    int i, actnum;
    struct sockaddr_in *addr;

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == sd)
    {
        perror("socket");
        exit(1);
    }

    conf.ifc_len = sizeof(struct ifreq) * NETCARDNUM;
    conf.ifc_req = req;

    ret = ioctl(sd, SIOCGIFCONF, &conf);
    if (-1 == ret) {
        perror("ioctl");
        exit(1);
    }

    actnum = conf.ifc_len / sizeof(struct ifreq);
    for (i = 0; i < actnum; i++) {
        addr = (struct sockaddr_in *)&(((conf.ifc_req)[i]).ifr_addr);
        if (strcmp(v[1], (conf.ifc_req)[i].ifr_name) == 0) {
            printf("%s:%s\n", (conf.ifc_req)[i].ifr_name, (char *)inet_ntoa(addr->sin_addr));
        }
    }

    close(sd);

    return 0;
}
