#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

int main(int c, char **v)
{
    if (c != 2)
    {
        perror("usage...\n");
        exit(1);
    }

    int i = 0, j = 0;
    while (environ[i])
    {
        if (strstr(environ[i], v[1]))
            break;
        i++;
    }


    while (*(*(environ + i) + j) != '=')
    {
        j++;
        putchar(*(*(environ + i) + j));
    }
    printf("1\n");
    while (*(*(environ + i) + j)!= '\n')
    {
        j++;
        putchar(*(*(environ + i) + j));
    }


    return 0;
}
