#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

class A
{
    public:
        void destory()
        {
            delete this;
        }
};


int main(int argc, char *argv[])
{
    A * a = new A();

    a->destory();

    return 0;
}
