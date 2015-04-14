#include <iostream>

class A{
    public:
static void show();
};

void A::show()
{
    std::cout<<"hello world"<<std::endl;
}

class B:public A{

};

int main(int argc, char *argv[])
{
    A a;
    a.show();

    B b;
    b.show();

    return 0;
}
