#include <stdio.h>
#include <stdlib.h>

const char * myindex(const char *src, int c)
{
    const char *s = src;
    if (s == NULL)
        return NULL;

    for (s; *s != '\0'; s++)
        if (*s == c)
            return s;
    return NULL;
}

int main(int c, char **v)
{
    if (c != 3)
        return 1;

    printf("%s\n", myindex(v[1], *v[2]));

    return 0;
}
