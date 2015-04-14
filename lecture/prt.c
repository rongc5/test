#include <stdio.h>

int main(int argc, char *argv[])
{
    int a = 6, b = 7 ;
    int *ptr = &a;

    printf("%p, %p, %p\n", ptr, &a, &ptr);

    ptr = &b;

    return 0;
}
