#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

void test(const string & t)
{
    printf("%s\n", t.c_str());
}

int main(int argc, char *argv[])
{
    test("hello");

    return 0;
}
