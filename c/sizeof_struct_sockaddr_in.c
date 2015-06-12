#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(void)
{
    struct sockaddr sa;

    printf("%d\n", sizeof(sa));

    return 0;
}
