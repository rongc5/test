#include <stdio.h>
#include <sys/socket.h> 
#include <netdb.h>
#include <stdlib.h>


int main(int c, char **v)
{
    if (c != 2){
        perror("usage...");
        return -1;
    }

    struct hostent *hp;
    char buf[16];
    char   *ptr, **pptr;

    hp = gethostbyname(v[1]);
    if (hp == NULL){
        fprintf(stderr, "%s\n", hstrerror(h_errno));
        return -1;
    }

    printf("off name:%s\n", hp->h_name);

    for (hp->h_aliases; *(hp->h_aliases); hp->h_aliases++)
        printf("alise name:%s\n", *(hp->h_aliases));

    pptr = hp->h_addr_list;
    for (; *pptr != NULL; pptr++){
        inet_ntop(hp->h_addrtype, *pptr, buf, sizeof(buf));
        printf("%s\n", buf);
    }

    printf("first:%s\n", inet_ntop(hp->h_addrtype, hp->h_addr, buf, 16));

    return 0;
}
