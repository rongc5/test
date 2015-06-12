#include <stdio.h>
#include <stdlib.h>

int mystrlen(const char *s)
{
    int count;
    if (s == NULL)
        return 0;

    for (count = 0, s; *s != '\0'; s++)
         count++;

    return count;
}


int main(int c, char **v)
{
    if (c != 2)
        return -1;

    printf("%d\n", mystrlen(v[1]));

    return 0;
}
