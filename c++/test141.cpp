#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

struct test {
    int a;
    string str;
};


int main(int argc, char *argv[])
{
    test a;
    a.str = "hello world";
    printf("%d  %d", sizeof(a), a.str.size());

    return 0;
}

