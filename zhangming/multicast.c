#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>

#define MCAST_ADDR "225.0.0.7"
#define LOCAL_ADDR "192.168.0.24"


int main(void)
{
    struct sockaddr_in addr;
    unsigned char buf[1024];
    int loop = 1;
    int sockfd;
    int addrlen;
    int ret;
    int i;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    

    printf("step1...\n");
    addr.sin_family = AF_INET;
    printf("step2...\n");
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));

    setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));

    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(MCAST_ADDR);
    mreq.imr_interface.s_addr = inet_addr(LOCAL_ADDR);

    setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
    
    addrlen = sizeof(addr);
    while (1)
    {
        printf("waiting...\n");
        ret = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &addrlen);
        printf("ret: %d\n", ret);
   		for (i = 0; i < ret; i++)
   	 	printf("%02x", buf[i]);
		sleep(2);
		printf("\n");
    }
}
