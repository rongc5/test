#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(int c, char **v)
{
    char *a="aBcddfefekr";
    char *b="AbCddfefekr";

    printf("%d\n", strcasecmp(a, b));

    return 0;
}
