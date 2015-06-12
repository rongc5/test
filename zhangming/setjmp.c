#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

jmp_buf env;

void test2(void)
{
    longjmp(env, 3);
}
void test1(void)
{
    test2();
}
void test(void)
{
    test1();
}

int main(void)
{
    int ret;
    volatile int val;
    printf("setjmp begin!\n");

    val = 3;
    ret = setjmp(env);
    if (0 == ret)
    {
        printf("call test!\n");
        val = 5;
        test();
    }
    else if (3 == ret)
    {
        printf("longjmp back!\n");
    }
    printf("val = %d\n", val);

    return 0;
}
