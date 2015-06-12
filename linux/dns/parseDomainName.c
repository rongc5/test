#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

int main(int c, char **v)
{
    if (c < 2){
        perror("usage!...");
        return 1;
    }

    char *service = NULL;
    char ipbuf[16];
    int ret, port;
    struct sockaddr_in * addr;
    if (c == 3)
        service = v[2];
    struct addrinfo *res = NULL, *cur = NULL, hint;

    memset(&hint, 0, sizeof(struct addrinfo));
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_protocol = 0;
    hint.ai_flags = AI_CANONNAME;

    ret = getaddrinfo(v[1], service, &hint, &res);
    if (ret) {
        fprintf(stderr, "%s\n", gai_strerror(ret));
        exit(1);
    }

    for (cur = res; cur != NULL; cur = cur->ai_next) {
        addr = (struct sockaddr_in *)cur->ai_addr;
        printf("%s(port:%d)\n", (char *)inet_ntop(AF_INET,
                    &addr->sin_addr, ipbuf, 16),
                ntohs(addr->sin_port));
    }

    freeaddrinfo(res);

    return 0;
}


