#include <stdio.h>
#include <stdlib.h>

const char * myindex(const char *src, int c)
{
    const char *s = src, *tmp = NULL;
    if (s == NULL)
        return NULL;

    for (s; *s != '\0'; s++)
        if (*s == c)
            tmp = s;
    return tmp;
}

int main(int c, char **v)
{
    if (c != 3)
        return 1;

    printf("%s\n", myindex(v[1], *v[2]));

    return 0;
}
