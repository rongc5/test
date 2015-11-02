#include <iostream>
#include <string.h>
#include <stdio.h>

using namespace std;

class A
{
    public:
        int a;
        char buf[12];
        char * ptr;
};

int main(int argc, char *argv[])
{
    A a, b;

    char buf[16] = {"zhangming"};
    a.a = 10;
    sprintf(a.buf, "%s", "hello world");
    a.ptr = buf;

    //b = a;
    memcpy(&b, &a, sizeof(b));

    printf("%d\n", sizeof(b));
    printf("%d\t%s\t%s\n", a.a, a.buf, a.ptr);

    return 0;
}
