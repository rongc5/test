#include <stdio.h>
#include <stdlib.h>

char * mystrstr(const char *src, const char * dst)
{
    char * ps = src;
    char * pd = dst;
    char * tmpsrc = NULL;
    char * tmpdst = NULL;

    if (src == NULL || dst == NULL)
        return NULL;

    for (ps; *ps != '\0'; ps++){
        for (tmpsrc = ps, tmpdst = pd; *tmpsrc != '\0' && *tmpdst != '\0' && *tmpsrc == *tmpdst; tmpsrc++, tmpdst++)
            ;
        if (*tmpdst == '\0')
            break;
    }

    return ps;
}

int main(int c, char **v)
{
    if (c != 3)
        return -1;

    printf("%s\n", mystrstr(v[1], v[2]));

    return 0;
}
