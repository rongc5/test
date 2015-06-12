#include <stdio.h>

int main(int argc, char *argv[])
{
    short int a = 1024;

    printf("%p, %p, %d\n", a, a>>8, a & 0x00ff);
    

    return 0;
}
