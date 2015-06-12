#include <stdio.h>

typedef struct{
    int b;
    char c[];
}DATA;

int main(void)
{
    DATA a;

    printf("%d\n", sizeof(a));

    return 0;
}
