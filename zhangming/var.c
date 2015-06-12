#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

int add(int num, ...)
{
    int sum = 0, i;
    va_list ap;

    va_start(ap, num);

    for (i = 0; i < num; i++)
    {
        sum += va_arg(ap, int);
    }

    va_end(ap);
    return sum;
}

int main(void)
{
    printf("sum = %d\n", add(7, 1, 2, 3, 4, 5, 6, 7));
    return 0;
}
