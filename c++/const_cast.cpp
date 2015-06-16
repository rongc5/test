#include <iostream>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int c = 99;
    const int a = c;

    int * b = const_cast<int *>(&a);
    *b = 199;
    printf("b = %d\n", a);

    return 0;
}
