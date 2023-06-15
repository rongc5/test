#include <stdio.h>

int main(int argc, char *argv[])
{
    short a = 0;
    char * p = &a;
    *p = 0x07;
    *(p+1) = 0x00;

    printf("%#06hx\n", a);

    return 0;
}
