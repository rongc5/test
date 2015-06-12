#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int *ptr;
    int a = 6;
    ptr = &a;

    printf("%d\n", ++*ptr);
    *ptr = 6;
    printf("%d\n", *ptr++);

    return 0;
}
