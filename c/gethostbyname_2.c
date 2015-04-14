#include <stdio.h>
#include <stdlib.h>
 #include <netdb.h>


int main(int c, char **v)
{
    if (c != 2){
        perror("usage...");
        return 1;
    }

    char *dst = NULL;
    dst = v[1];
    struct hostent *hp;
    hp = gethostbyname2(dst, AF_INET);
    if (!hp) {
        fprintf(stderr, "arping: unknown host %s\n", dst);
        return -1;
    }

    printf("%s\n", hp->h_name);
    return 0;
}
