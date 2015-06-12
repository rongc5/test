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

int main(int c, char **v)
{
    if (c != 2) {
        fprintf(stderr, "usage...\n");
        exit(1);
    }

    int sd, ret;
    struct ifreq req;
    struct sockaddr_in *addr;

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd == -1) {
        perror("socket");
        exit(1);
    }

    strcpy(req.ifr_name, v[1]);

    ret = ioctl(sd, SIOCGIFADDR, &req);
    if (ret == -1) {
        perror("iotcl");
        exit(1);
    }

    addr = (struct sockaddr_in *)&(req.ifr_addr);

    printf("%s:%s\n", v[1], (char *)inet_ntoa(addr->sin_addr));
    close(sd);
    return 0;
}
