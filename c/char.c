#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char c;

    for (c = 0; c < 127; c++)
        printf("%c\n", c);

    return 0;
}
