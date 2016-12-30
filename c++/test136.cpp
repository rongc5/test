#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

class test {
public:
    int id;
    int len;
    string buf;
};


int main(int argc, char *argv[])
{
    test t;
    t.id = 1;
    t.buf = "hello world";

    printf("%d  %d\n", sizeof(t), t.buf.size());
    
    return 0;
}
