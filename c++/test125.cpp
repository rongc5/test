#include <iostream>

using namespace std;

class A
{
    public:
        A();
        void init();
        void display();
    private:
        int a;
};

void A::init()
{
    this->a = 99;
}

void A::display()
{
    cout << this->a << "\n";
}

A::A()
{
    this->init();
}

int main(int argc, char *argv[])
{
    A a;
    a.display();

    return 0;
}
