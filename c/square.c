#include <stdio.h>

#define SQA(a) ((a)*(a))

int main(void)
{
    int a = 5;
    printf("%d\n", SQA(a++));

    printf("a:%d\n", a);

    return 0;
}
