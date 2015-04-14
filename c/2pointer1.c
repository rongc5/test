#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char **p;
    char *buf = "hello";

    p = &buf;

    printf("%s\n", *p);
    
    return 0;
}
