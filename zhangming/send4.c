#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

typedef struct send_udp
{
    struct iphdr i_hdr;
    struct udphdr u_hdr;
    char data[32];
}__attribute__((packed)) udp_raw_t;

void bild

int main(void)
{
    int sd, ret;
    struct sockaddr_in dst;
    socklen_t len;
    udp_raw_t packet;

    len = sizeof(dst);
    dst.sin_family = AF_INET;
    dst.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.1", &dst.sin_addr);

    sd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (-1 == sd)
    {
        perror("socket");
        exit(1);
    }

    strcpy(packet.data, "hello world");
    build_ip_header(&)

}
