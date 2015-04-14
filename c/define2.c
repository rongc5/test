#include <stdio.h>

#define DDD(x) x###_1

int main(void)
{
    DDD(5);

    return 0;
}
