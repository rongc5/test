#include <stdio.h>

int main(int c, char **v)
{
    int i;

    for (i = 0; i < c; i++)
        printf("%d:%s\n", i, v[i]);

    return 0;
}
