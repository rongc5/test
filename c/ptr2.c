#include <stdio.h>
#include <stdlib.h>

int add(int a, int b)
{
    return a+b;
}


int main(int argc, char *argv[])
{
    /*int (int a, int b) *ptr = &add;*/
    int (*ptr)(int a, int b) = &add;
    
    printf("%d\n", ptr(1, 5));

    return 0;
}
