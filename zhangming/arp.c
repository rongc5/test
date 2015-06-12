#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <netinet/if_ether.h>

typedef struct arp_header
{
    struct ether_arp arp_hdr;
}__attribute__((packed)) arp_hdr_t;

int main(void)
{
    setuid(0);
    int sd, ret;
    struct sockaddr_ll src;
    socklen_t len;
    struct arp_header buf;
    char ipbuf[16];

    len = sizeof(src);
    memset(&buf, 0, sizeof(buf));

    sd = socket(PF_PACKET, SOCK_DGRAM, htons(ETH_P_ARP));
    if (sd == -1)
    {
        perror("socket");
        exit(1);
    }

    while (1)
    {
        ret = recvfrom(sd, &buf, sizeof(buf), 0, (struct sockaddr *)&src, &len);
        if (-1 == ret)
        {
            perror("recvfrom");
        }

        if (ntohs(buf.arp_hdr.arp_op) == ARPOP_REQUEST)
        {
            printf("%s ask \n %s's mac, and his mac addr = %s\n",
                    (char *)inet_ntoa(
                        *((uint32_t *)(buf.arp_hdr.arp_spa))),
                    (char *)inet_ntop(AF_INET,
                        (uint32_t *)(buf.arp_hdr.arp_tpa), ipbuf, 16),
                    (char *)ether_ntoa(buf.arp_hdr.arp_sha));
        }
    }
}
