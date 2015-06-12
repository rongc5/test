#include <stdio.h>

int main(void)
{
    union V{
        struct X{
            unsigned char s1:2;
            unsigned char s2:3;
            unsigned char s3:3;
        }x;
        unsigned char c;
    }v;

    v.c=100;
    printf("%o, %d\n", v.c, v.x.s3);

    return 0;
}
