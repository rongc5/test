#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <sys/types.h>
#include <arpa/inet.h>

#if 0
int getaddrinfo(const char * node, const char *service, 
        const struct addrinfo *hints, struct addrinfo **res);
struct addrinfo {
    int              ai_flags;
    int              ai_family;
    int              ai_socktype;
    int              ai_protocol;
    size_t           ai_addrlen;
    struct sockaddr *ai_addr;
    char            *ai_canonname;
    struct addrinfo *ai_next;
};

#endif
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "usage...\n");
        exit(1);
    }

    int ret;
    struct addrinfo *res, *cur, hint;
    struct sockaddr_in *addr;
    char ipbuf[16];

    bzero(&hint, sizeof(hint));
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_DGRAM;
    hint.ai_protocol = 0;
    hint.ai_flags = AI_CANONNAME;

    //ret = getaddrinfo(argv[1], NULL, &hint, &res);
    ret = getaddrinfo(argv[1], NULL, NULL, &res);
    if (-1 == ret)
    {
        fprintf(stderr, "%s\n", gai_strerror(ret));
        exit(1);
    }

    for (cur = res; cur != NULL; cur = cur->ai_next)
    {
        addr = (struct sockaddr_in *)cur->ai_addr;
        //printf("%s(port:%d)(%s)\n", (char *)inet_ntop(AF_INET, &addr->sin_addr, ipbuf, 16), ntohs(addr->sin_port), cur->ai_canonname);
        inet_ntop(AF_INET, &addr->sin_addr, ipbuf, 16);
        printf("ip:%s\n", ipbuf);
    }

    freeaddrinfo(res);
    exit(0);
}
