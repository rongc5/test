#include <stdio.h>
#include <string.h>

struct stu
{
    int a;
    int b;
};

int main(int argc, char *argv[])
{
    unsigned long long c = 0;
    int a = 168, b = 3;
    
    c = b;
    c = c << 32;
    
    printf("%llu\n", c);
    c +=  a;

    printf("%llu\n", c);

    /*struct stu data;
    data.*/

    return 0;
}
