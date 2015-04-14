#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>

typedef struct pseudo_hdr
{
    uint32_t src;
    uint32_t dst;
    uint8_t zero;
    uint8_t protocol;
    uint16_t len;
}__attribute__((packed)) pse_hdr_t;

typedef struct send_udp
{
    struct iphdr i_hdr;
    struct udphdr u_hdr;
    char data[32];
}__attribute__((packed)) udp_raw_t;

typedef struct
{
    pse_hdr_t p_hdr;
    struct udphdr u_hdr;
    char data[32];
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

    if (size == 1)
        sum += *((unsigned char *)base);
    while (sum >> 16)
        sum = (sum >> 16) + (sum & 0xffff);

    return ~sum;
}

void cal_udp_checksum(udp_raw_t *hdr)
{
    char *base = (char *)hdr;
    struct iphdr *i_hdr = &(hdr->i_hdr);
    struct udphdr *u_hdr = &(hdr->u_hdr);

    csum_t c_buf;

    c_buf.p_hdr.src = i_hdr->saddr;
    c_buf.p_hdr.dst = i_hdr->daddr;
    c_buf.p_hdr.zero = 0;
    c_buf.p_hdr.protocol = i_hdr->protocol;
    c_buf.p_hdr.len = u_hdr->len;

    memcpy((char *)(&c_buf) + sizeof(pse_hdr_t), base + sizeof(struct iphdr), sizeof(csum_t) - sizeof(pse_hdr_t));

    u_hdr->check = csum((char *)(&c_buf), sizeof(csum_t));
    printf("udp check = %#x\n", u_hdr->check);
}

void build_ip_header(udp_raw_t *hdr)
{
    struct iphdr *p_hdr = &(hdr->i_hdr);

    p_hdr->version = 4;
    p_hdr->ihl = 5;
    p_hdr->tos = 0;
    p_hdr->tot_len = htons(sizeof(udp_raw_t));
    p_hdr->id = htons(100);
    p_hdr->frag_off = htons(0);
    p_hdr->ttl = 64;
    p_hdr->protocol = IPPROTO_UDP;
    p_hdr->check = 0;
    p_hdr->saddr = inet_addr("7.8.9.6");
    p_hdr->daddr = inet_addr("192.168.1.116");

    p_hdr->check = csum((char *)p_hdr, sizeof(struct iphdr));
    printf("ip check = %#x\n", p_hdr->check);
}

void build_udp_header(udp_raw_t *hdr)
{
    struct udphdr *p_hdr = &(hdr->u_hdr);

    p_hdr->source = htons(7878);
    p_hdr->dest = htons(9999);
    p_hdr->check = htons(0);
    p_hdr->len = htons(sizeof(udp_raw_t) - sizeof(struct iphdr));
    cal_udp_checksum(hdr);
}

int main(void)
{
    int sd, ret;
    struct sockaddr_ll dst;
    socklen_t len;
    udp_raw_t packet;
    int opt;

    memset(&dst, 0, sizeof(dst));
    len = sizeof(dst);
    dst.sll_family = AF_PACKET;
    dst.sll_protocol = htons(ETH_P_IP);
    dst.sll_ifindex = if_nametoindex("eth1");
    dst.sll_halen = ETH_ALEN;
    memcpy(dst.sll_addr, (void *)ether_aton("70:f1:a1:34:f5:50"), ETH_ALEN);

    sd = socket(AF_PACKET, SOCK_DGRAM, ETH_P_IP);
    if (-1 == sd)
    {
        perror("sendto");
        exit(1);
    }

    strcpy(packet.data, "hello world");
    build_ip_header(&packet);
    build_udp_header(&packet);

    ret = sendto(sd, &packet, sizeof(packet), 0, (struct sockaddr *)&dst, len);

    if (-1 == ret)
    {
        perror("sendto");
        exit(1);
    }

    close(sd);
    exit(0);
}
