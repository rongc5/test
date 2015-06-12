#include <stdio.h>

int main() 
{
    int a = 10, b;

    __asm__("movl %1, %%eax\n\t"
            "movl %%eax, %0\n\t"
            :"=r"(b)        /* output */
            :"r"(a)         /* input */
            :"%eax"         /* clobbered register */
           );
    printf("Result: %d, %d\n", a, b);
    return 0;
}
