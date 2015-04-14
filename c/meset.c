#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int c, char **v)
{
    char buf[33];
    
    memset(buf, 5, 33);
    
    printf("buf: %s\n", buf);
    for (int i = 0; i < 33; i++)
    printf("%d", buf[i]);

    printf("\n");
    return 0;
}
