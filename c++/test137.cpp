#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;


struct A {
    int len;
};

struct B: public A {
    int id;
    string str;
};

int main(int argc, char *argv[])
{
    A a;
    B b;

    printf("%d  %d\n", sizeof(a), sizeof(b));

    return 0;
}
