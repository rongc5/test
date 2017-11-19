#include <string>
#include <iostream>
#include <stdio.h>

using namespace std;

struct T1
{
    int a;
    string  b;
    string c;
};


int main(int argc, char *argv[])
{
    T1 t;

    t.a = 1;
    t.b = "hello world, 123";

    printf("%d\n", sizeof(t));

    return 0;

}
