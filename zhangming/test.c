#include <stdio.h>

int test1(void)
{
L1:
    printf("go back!\n");
    return 0;
}

void test(void)
{
    printf("goto main!\n");
    goto L1;
}


int main(void)
{
    test();
}
