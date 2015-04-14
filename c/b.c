#include <stdio.h>

int b = 5;

int main(int argc, char *argv[])
{
    int b = 1, a = 1;
    
    if (a)
    {
        extern int b;
        printf("%d, %d\n", b, ++b);
    }

    return 0;
}
