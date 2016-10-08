#include <stdio.h>

int main(int argc, char *argv[])
{
    int a[3][5];

    printf("%d\n", sizeof(a)/sizeof(a[0]));

    return 0;
}
