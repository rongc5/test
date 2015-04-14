#include <stdio.h>
#include <string.h>
#include <sys/types.h>  
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <sys/time.h>
#include <net/if.h>
#include <linux/sockios.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>





typedef struct send_hdr{
    struct iphdr i_hdr;
    struct tcphdr t_hdr;
}syn_t;

struct pse_hdr {
    uint32_t	saddr;
    uint32_t	daddr;
    uint8_t		pad;
    uint8_t		protocol;
    uint16_t	len;
};

typedef struct 
{
    struct pse_hdr p_hdr;
    struct tcphdr t_hdr;
}csum_t;


unsigned short csum(char *buf, int size)
{
    unsigned short *base = (unsigned short *)buf;
    unsigned long sum = 0;

    while (size > 1)
    {
        sum += *(base++);
        if (sum >> 16)
            sum = (sum >> 16) + (sum & 0xffff);
        size -= 2;
    }

    if (1 == size)
        sum += *((unsigned char *)base);
    while (sum >> 16)
        sum = (sum >> 16) + (sum & 0xffff);

    return ~sum;
}

void cal_tcp_checksum(syn_t *hdr)
{
    char *base = (char *)hdr;
    struct iphdr *i_hdr = &(hdr->i_hdr);
    struct tcphdr *t_hdr = &(hdr->t_hdr);

    csum_t c_buf;

    memset(&c_buf, 0, sizeof(c_buf));
    c_buf.p_hdr.saddr = i_hdr->saddr;
    c_buf.p_hdr.daddr = i_hdr->daddr;
    c_buf.p_hdr.pad = 0;
    c_buf.p_hdr.protocol = i_hdr->protocol;
    c_buf.p_hdr.len = htons(sizeof(struct tcphdr));

    //memcpy((char *)(&c_buf) + sizeof(struct pse_hdr), base + sizeof(struct iphdr), sizeof(csum_t) - sizeof(struct pse_hdr));
    //printf("%d  %d\n", sizeof(struct pse_hdr), sizeof(struct iphdr));
    memcpy(&c_buf.t_hdr, &hdr->t_hdr, sizeof(struct tcphdr));

    t_hdr->check = csum((char *)(&c_buf), (sizeof(csum_t)));
    //printf("tcp check = %#x\n", t_hdr->check);
}

void build_ip_header(syn_t *hdr, struct sockaddr_in *dstaddr, struct sockaddr_in *srcaddr)
{
    struct iphdr *p_hdr = &(hdr->i_hdr);

    p_hdr->version = 4;
    p_hdr->ihl = 5;
    p_hdr->tos = 0;
    p_hdr->tot_len = htons(sizeof(syn_t));
    p_hdr->id = htons((uint32_t)rand());
    p_hdr->frag_off = 0;
    p_hdr->ttl = 64;
    p_hdr->protocol = IPPROTO_TCP;
    p_hdr->check = 0;
    //p_hdr->saddr = (uint32_t)(rand());
    p_hdr->saddr = srcaddr->sin_addr.s_addr;
    p_hdr->daddr = dstaddr->sin_addr.s_addr;

    p_hdr->check = csum((char *)p_hdr, sizeof(struct iphdr));
    //printf("ip check = %#x\n", p_hdr->check);
}

void build_tcp_header(syn_t *hdr, struct sockaddr_in *dstaddr, uint16_t dport)
{
    struct tcphdr *p_hdr = &(hdr->t_hdr);

    p_hdr->source = htons((uint16_t)(rand()));
    p_hdr->dest = htons(dport);
    p_hdr->seq = htonl(rand());
    p_hdr->ack_seq = htonl(0);
    p_hdr->res1 = 0;
    p_hdr->doff = sizeof(struct tcphdr)/4;
    p_hdr->fin = 0;
    p_hdr->syn = 1;
    p_hdr->rst = 0;
    p_hdr->psh = 0;
    p_hdr->ack = 0;
    p_hdr->urg = 0;
    p_hdr->res2 = 0;
    p_hdr->window = htons(8192);
    p_hdr->check = htons(0);
    p_hdr->urg_ptr = htons(0);
}

int check_syn_ack(syn_t *spacket, syn_t *rpacket)
{
    char buf[16];
    struct tcphdr *ps_thdr = &(spacket->t_hdr);
    struct tcphdr *pr_thdr = &(rpacket->t_hdr);

    struct iphdr *ps_ihdr = &(spacket->i_hdr);
    struct iphdr *pr_ihdr = &(rpacket->i_hdr);

    if (pr_thdr->syn != 1 || pr_thdr->ack != 1) {
        //printf("this is not the expected ack + syn!\n");
        if (pr_thdr->ack == 1 && pr_thdr->rst == 1)
		printf("the port is not listening!\n");
        return -1;
    }

    //printf("%u, %u\n",  ntohl(ps_thdr->seq), ntohl(ps_thdr->ack_seq));
    //printf("%u, %u\n",  ntohl(pr_thdr->seq), ntohl(pr_thdr->ack_seq));

    if (ntohl(ps_thdr->seq) != (ntohl(pr_thdr->ack_seq) - 1)){
        //printf("this is not the expected ack NO.!\n");
        return -2;
    }
    if (ps_ihdr->daddr != pr_ihdr->saddr){
        //printf("this is not the expected ip!\n");
        return -3;
    }

    if (ps_thdr->source != pr_thdr->dest){
        //printf("this is not the expected port!\n", ps_thdr->source);
        return -4;
    }

    return 0;
}

void usage()
{
    fprintf(stderr, "exampe:./app 192.168.3.115 7881\n");
}

int localIp(char *ip)
{
    int s;
    struct ifconf conf;
    struct ifreq *ifr;
    char buff[BUFSIZ];
    int num;
    int i;

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
            memcpy(ip,inet_ntoa(sin->sin_addr),16);
        }
        ifr++;
    }
} 

int main(int c, char **v)
{
    int sd, ret;
    struct sockaddr_in dst, src;
    socklen_t len;
    syn_t spacket, rpacket;
    int opt = 1;
    char hostip[16];
    localIp(hostip);
    time_t uptime = time(NULL);
    if (c != 3){
        usage();
        return -1;
    }

    len = sizeof(dst);
    dst.sin_family = AF_INET;
    dst.sin_port = htons(atoi(v[2]));
    inet_pton(AF_INET, v[1], &dst.sin_addr);
    inet_pton(AF_INET, hostip, &src.sin_addr);

    sd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (-1 == sd){
        perror("socket");
        return -1;
    }	

    setsockopt(sd, IPPROTO_IP, IP_HDRINCL, &opt, sizeof(opt));

        build_ip_header(&spacket, &dst, &src);
        build_tcp_header(&spacket, &dst, atoi(v[2]));
        cal_tcp_checksum(&spacket);

        ret = sendto(sd, &spacket, sizeof(spacket), 0, (struct sockaddr *)&dst, len);
        ret = recvfrom(sd, &rpacket, sizeof(rpacket), 0, (struct sockaddr *)&dst, &len);
        if (!check_syn_ack(&spacket, &rpacket)){
		printf("the IP:%s and the port:%d is ok\n", v[1], atoi(v[2]));
	}else {
		printf("the IP:%s and the port:%d is not ok\n", v[1], atoi(v[2]));
	}
	
    close(sd);
    return 0;
}
