#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char **p;
    char *buf = malloc(20);

    memcpy(buf, "helo, world!", 20);
    p = &buf;
    //buf[10] = '\0';
    //printf("%s\n", *p);
   p[1] = NULL;
    while (buf)
    printf("%c ", *buf++);
    printf("\n");
    
    return 0;
}
