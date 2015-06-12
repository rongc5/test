#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char *p;

    printf("%d, %d, %d, %d, %d\n", *p++, *p++, *p++, *p++, *p++);

    return 0;
}
