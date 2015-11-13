#include <stdio.h>

int main(int argc, char *argv[])
{
    int a = 5;

    int b = 3;

    a = a| b;
    printf("%d,\t%d\n", a, a&b);

    return 0;
}

