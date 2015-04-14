#include <stdio.h>

int main(void)
{
    int a = 3;

    printf("%p, %p\n", &a, &a + 1);

    return 0;
}
