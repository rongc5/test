#include <stdio.h>

int main(int argc, char *argv[])
{
    int b[] = {1, 2, 3, 4, 5, 6};
    const int * ptr = b;
    ptr++;
    printf("%d\n", *ptr);
    *ptr = 10;

    return 0;
}
