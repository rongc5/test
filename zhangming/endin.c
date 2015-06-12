#include <stdio.h>

union endin{
    int a;
    char c;
};

int main(void)
{
    union endin x;
    x.a = 0x12345678;
    if (x.c == 0x78)
        printf("小端\n");
    else if (x.c == 0x12)
        printf("大端\n");

    return 0;
}
