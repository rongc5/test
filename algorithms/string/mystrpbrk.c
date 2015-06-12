#include <stdio.h>
#include <stdlib.h>

char * mystrpbrk(const char *s, const char *accept)
{
    if (s == NULL || accept == NULL)
        return NULL;

    char *ps = s;
    char *pa = accept;
    char *tmpa = NULL;

    for (ps; *ps != '\0'; ps++){
        for (ps, tmpa = pa; *ps != '\0' && *tmpa != '\0' && *tmpa != *ps; tmpa++)
            ;
        if (*tmpa != '\0' && *ps != '\0')
            break;
    }
    return ps;
}

int main(int c, char **v)
{
    if (c != 3)
        return -1;

    printf("%s", mystrpbrk(v[1], v[2]));

    return 0;
}

