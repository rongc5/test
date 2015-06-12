#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    short b = 1;
    //memset(&b, 0, 4);

    char * p = (char *)&b;
    *p = 0;
    if (p == NULL)
    printf("hello %s\n", p);
    return 0;
}
