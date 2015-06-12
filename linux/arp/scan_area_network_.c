#include <stdlib.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <linux/sockios.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>

#include <sys/signal.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <net/if_arp.h>
#include <sys/uio.h>

#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>


typedef struct  send_arping{
    struct ethhdr ehdr;
    struct arphdr ahdr;
#if 0
    unsigned char arp_dst_hw[6];
    unsigned char arp_src_hw[6];
    unsigned short arp_type;
    unsigned short hw_type;
    unsigned short pro_type;
    unsigned char hw_len;
    unsigned char pro_len;
    unsigned short op;
#endif 
    unsigned char src_hw[6];
    struct in_addr src_ip;
    unsigned char dst_hw[6];
    struct in_addr dst_ip;
    unsigned char trailer[18];
}__attribute__((packed)) send_arping_t;


void print_hex(unsigned char *p, int len)
{
    int i;
    for (i=0; i<len; i++) {
        printf("%02X", p[i]);
        if (i != len-1)
            printf(":");
    }
}


int recv_pack(int sfd, struct in_addr dst, struct in_addr src)
{
    send_arping_t rbuf;
    int ret;
    char ipbuf[16];
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 200000;
    fd_set fds;
    static struct in_addr tmp;

    while (1){
        FD_ZERO(&fds);
        FD_SET(sfd, &fds);
        ret = select(sfd + 1, &fds, 0, 0, &tv);
        if (-1 == ret || ret == 0){
            //printf("timeout!, the dst  %s    meybe is not living\n", inet_ntop(AF_INET, &dst.s_addr, ipbuf, sizeof(ipbuf)));
            return -1;
        }

        ret = recvfrom(sfd, &rbuf, sizeof(rbuf), 0, NULL, NULL);
        if (-1 == ret){
            perror("recvfrom:");
            return -1;
        }

        //printf("recvfrom:%d\n", ret);
        //if (rbuf.ahdr.ar_op != htons(ARPOP_REQUEST) && rbuf.ahdr.ar_op != htons(ARPOP_REPLY))
        if (rbuf.ahdr.ar_op != htons(ARPOP_REPLY)){
            return -1;
        }

        if (!memcmp(&rbuf.src_ip, &tmp, sizeof(struct in_addr))){
                return 0;
                //printf("timeout!, the dst  %s    meybe is not living\n", inet_ntop(AF_INET, &dst.s_addr, ipbuf, sizeof(ipbuf)));
        }

#if 1
        if (memcmp(&rbuf.src_ip, &dst, sizeof(struct in_addr))){
                return -1;
                //printf("timeout!, the dst  %s    meybe is not living\n", inet_ntop(AF_INET, &dst.s_addr, ipbuf, sizeof(ipbuf)));
        }


        tmp = dst;
        printf("replay from:%s  ", inet_ntoa(rbuf.src_ip));
        print_hex(rbuf.src_hw, ETH_ALEN);
        printf("\n");
#endif
        break;
    }

    return 0;
}

void package(char *dst, unsigned char signal_value, int len)
{
    int i = 0;
    for (i; i < len; i++)
        *dst++ = signal_value;
}

int send_pack(int sfd, char *src_mac, int ifindex, struct in_addr src, struct in_addr dst)
{
    send_arping_t sbuf;
    int ret;
    struct sockaddr_ll to;
    memset(&sbuf, 0, sizeof(send_arping_t));

    package(sbuf.ehdr.h_dest, 0xFF, ETH_ALEN);
    memcpy(sbuf.ehdr.h_source, src_mac, ETH_ALEN);
    sbuf.ehdr.h_proto = htons(ETH_P_ARP);
    sbuf.ahdr.ar_hrd = htons(ARPHRD_ETHER);
    sbuf.ahdr.ar_pro = htons(ETH_P_IP);
    sbuf.ahdr.ar_hln = ETH_ALEN;
    sbuf.ahdr.ar_pln = 4;
    sbuf.ahdr.ar_op = htons(ARPOP_REQUEST);

    memcpy(sbuf.src_hw, src_mac, ETH_ALEN);    
    sbuf.src_ip = src;
    package(sbuf.dst_hw, 0xFF, ETH_ALEN);
    sbuf.dst_ip = dst;
    package(sbuf.trailer, htons(0), sizeof(sbuf.trailer));

    to.sll_family = AF_PACKET;
    memcpy(to.sll_addr, sbuf.ehdr.h_dest, ETH_ALEN);
    to.sll_halen = ETH_ALEN;
    to.sll_ifindex = ifindex;

    ret = sendto(sfd, &sbuf, sizeof(sbuf), 0, (struct sockaddr*)&to, sizeof(to));
    if (ret != sizeof(sbuf)) {
        perror("sendto:");
        return -1;
    }

    return 0;
}

char *gethostip(int sd, char *device, int *ifindex, char *ipstr, size_t iplen)
{
    if (ipstr == NULL)
        return NULL;
    int ret;
    struct ifconf conf;
    struct ifreq *ifr;
    struct sockaddr_in *saddr = NULL;
    struct ifreq req; 
    int num;
    int i;

    memset(&req, 0, sizeof(struct ifreq));
    char buff[BUFSIZ];
    conf.ifc_len = BUFSIZ;
    conf.ifc_buf = buff;

    ioctl(sd, SIOCGIFCONF, &conf);
    num = conf.ifc_len / sizeof(struct ifreq);
    ifr = conf.ifc_req;


    for(i=0;i < num;i++)
    {
        saddr = (struct sockaddr_in *)(&ifr->ifr_addr);

        ioctl(sd, SIOCGIFFLAGS, ifr);
        if(((ifr->ifr_flags & (IFF_LOOPBACK | IFF_NOARP)) == 0) && (ifr->ifr_flags & IFF_UP))
        {		
            inet_ntop(AF_INET, &saddr->sin_addr, ipstr, iplen);
            //printf("%s:	%s\n", ifr->ifr_name, inet_ntop(AF_INET, &saddr->sin_addr, ipstr, iplen));
            memcpy(device, ifr->ifr_name, strlen(ifr->ifr_name) + 1);
        }
        ifr++;
    }	

    memcpy(req.ifr_name, device, strlen(device));		
    ioctl(sd, SIOCGIFINDEX, &req);
    *ifindex = req.ifr_ifindex;	

    return ipstr;
}


char *gethostmac(int sd, char *device, char *pmac)
{
    int ret;
    struct ifreq req;
    struct sockaddr_in *saddr = NULL;
    memset(&req, 0, sizeof(req));
    memcpy(req.ifr_name, device, strlen(device));

    memset(pmac, 0, ETH_ALEN);
    ret = ioctl(sd, SIOCGIFHWADDR, &req);
    if (-1 == ret){
        perror("ioctl");
        exit(1);
    }
    memcpy(pmac, req.ifr_hwaddr.sa_data, ETH_ALEN);
    return pmac;
}

void init_sfd(int sfd, int ifindex)
{
    struct sockaddr_ll from;

    from.sll_family = AF_PACKET;
    from.sll_ifindex = ifindex;
    from.sll_protocol = htons(ETH_P_ARP);
    if (bind(sfd, (struct sockaddr*)&from, sizeof(struct sockaddr_ll)) == -1) {
        perror("bind");
        exit(2);
    }
}


char * packed_for_ip(char * ipsrc, char * iptmp)
{
    char *ipdst = iptmp;

    int j = 0;

    while (*ipsrc != '\0' && j != 3){
        *iptmp = *ipsrc;
        if (*ipsrc == '.')
            j++;
        iptmp++;
        ipsrc++;
    }
    *iptmp ='\0';

    return ipdst;

}

int main(int c, char **v)
{
    int sfd, ret, ifindex;
    char ipsrc[16], ipdst[16], iptmp[16];
    char device[12], src_mac[ETH_ALEN];
    struct in_addr src, dst;
    struct sockaddr_ll from;
    int i = 1, j = 0;

    setuid(0);
    sfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (sfd == -1){
        perror("socket:");
        return 1;
    }
    gethostip(sfd, device, &ifindex, ipsrc, sizeof(ipsrc));
    inet_pton(AF_INET, ipsrc, &src.s_addr);
    gethostmac(sfd, device, src_mac);

    init_sfd(sfd, ifindex);

    packed_for_ip(ipsrc, iptmp);

    while (1){
        if (i >= 1 && i <= 254){
            sprintf(ipdst, "%s%d", iptmp, i);
            i++;
        }else {
            break;
        }
        inet_pton(AF_INET, ipdst, &dst.s_addr);
        if (!memcmp(&src, &dst, sizeof(struct in_addr))){
            printf("replay from:%s  ", ipsrc);
            print_hex(src_mac, ETH_ALEN);
            printf("\n");
            continue;
        }

        for (j = 0; j < 10; j++){
            send_pack(sfd, src_mac, ifindex, src, dst);
            if (!recv_pack(sfd, dst, src))
                break;
        }
    }

    return 0;
}
