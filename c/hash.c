#include <stdio.h>

unsigned short hash(unsigned short key)
{
    return (key>>1)%256;
}

int main(void)
{
    int i;
    i = hash(16);

    printf("%d\n", i);

    return 0;
}
