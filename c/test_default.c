#include <stdio.h>
#include <string.h>

int test(int a, int b)
{
    printf("%d %d\n", a, b);

    return 0;
}

int main(void)
{
    test(3, 7);
    test(2, 9);
}
