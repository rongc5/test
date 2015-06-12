#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int c, char **v)
{
    if (c != 2)
    {
        fprintf(stderr, "usage...\n");
        return -1;
    }

    printf("crypt:%s\n", (char *)crypt(v[1], "2z"));
 
    return 0;
}
