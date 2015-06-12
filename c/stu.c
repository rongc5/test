#include <stdio.h>

struct s1
{
    int i: 2;
    int j: 4;
    int a: 3;
//    double b;
}__attribute__((packed));

int main(void)
{
    printf("%d\n", sizeof(struct s1));

    return 0;
}
