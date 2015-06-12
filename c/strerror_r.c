#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int c, char **v)
{
    char buf[64];
    
    int sfd;
    write(sfd, "hello", 9);
    strerror_r(errno, buf, 64);
    printf("%s\n %s\n", strerror(errno), buf);
    return 0;
}
