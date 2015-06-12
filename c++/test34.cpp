#include <iostream>

class A{
    public:
        void show();
};

void A::show()
{
    std::cout<<__func__<<std::endl;
}

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
