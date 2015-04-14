#include <stdio.h>

int main(void)
{
    struct A{
        char t:4;
        char k:4;
        unsigned short i:8;
        unsigned long m;
    };

    printf("%d\n", sizeof (struct A));

    return 0;
}
