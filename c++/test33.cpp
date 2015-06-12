#include <iostream>

class A{
    public:
        static int x;
};

int A::x = 1;

class B:public A{
    public:
        static int x;
};

int B::x = 2;

int main(int argc, char *argv[])
{
    A a;
    std::cout<<a.x<<std::endl;

    B b;
    std::cout<<b.x<<a.x<<std::endl;

    return 0;
}
