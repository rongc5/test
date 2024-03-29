#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <linux/sockios.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
//#include "localIp.h"

int localIp(char *ip)
{
    int s;
    struct ifconf conf;
    struct ifreq *ifr;
    char buff[BUFSIZ];
    int num;
    int i;

    printf("%d\n", BUFSIZ);
    s = socket(PF_INET, SOCK_DGRAM, 0);
    conf.ifc_len = BUFSIZ;
    conf.ifc_buf = buff;

    ioctl(s, SIOCGIFCONF, &conf);
    num = conf.ifc_len / sizeof(struct ifreq);
    ifr = conf.ifc_req;

    struct sockaddr_in *sin = NULL;
    for(i=0;i < num;i++)
    {
        sin = (struct sockaddr_in *)(&ifr->ifr_addr);

        ioctl(s, SIOCGIFFLAGS, ifr);
        if(((ifr->ifr_flags & IFF_LOOPBACK) == 0) && (ifr->ifr_flags & IFF_UP))
        {
            printf("%s: %s\n", ifr->ifr_name, inet_ntoa(sin->sin_addr));
        }
        ifr++;
    }
    memcpy(ip,inet_ntoa(sin->sin_addr),16);
} 

int main(void)
{
    char ip[16];
    localIp(ip);

    return 0;
}
