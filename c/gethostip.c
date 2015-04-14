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
#include <arpa/inet.h>

int main(int c, char **v)
{

    int sd, ret;
    struct ifreq req;
    struct sockaddr_in *saddr;
    char ipbuf[16];
    memset(&req, 0, sizeof(struct ifreq));
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == sd){
        perror("socket");
        exit(1);
    }
    
    memcpy(req.ifr_name, "eth0", sizeof("eth0"));
    ret = ioctl(sd, SIOCGIFADDR, &req);
    if (-1 == ret) {
        perror("ioctl");
        exit(1);
    }
    
    saddr = (struct sockaddr_in *)&req.ifr_addr;

    printf("localip is:%s\n", inet_ntop(AF_INET, &saddr->sin_addr, ipbuf, sizeof(ipbuf)));
    
    close(sd);

    return 0;
}
