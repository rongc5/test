#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int c, char **v)
{
    if (c != 2)
    {
        perror("usage..");
        exit(1);
    }

    int d = access(v[1], F_OK);

    if (!d)
    {
        printf("存在\n");
    }

    return 0;
}
