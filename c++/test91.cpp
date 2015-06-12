#include <iostream>

#include <stdio.h>
//#include <stdlib.h>

int main(int argc, char *argv[])
{
    try {
        int a = 6;
        throw &a;
    }
    catch (int * p)
    {
        printf("%d\n", *p);
    }

    return 0;
}
