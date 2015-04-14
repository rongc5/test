#include <stdio.h>
#include <stdlib.h>

int mystrcmp(const char *s1, const char *s2)
{
    for (; *s1 != '\0' && *s2 != '\0' && *s1 == *s2; s1++, s2++)
        ;
    return *s1 - *s2;
}

int main(int c, char **v)
{
    if (c != 3)
        return -1;

    printf("%d\n", mystrcmp(v[1], v[2]));

    return 0;
}
