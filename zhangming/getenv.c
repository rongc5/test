#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int c, char **v)
{
    printf("%s:%s\n", v[1], getenv(v[1]));

    return 0;
}
