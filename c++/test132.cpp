#include <iostream>

using namespace std;

class A
{
    public:
        virtual void display()
        {
            cout << "I am A\n";
        }
        virtual ~A()
        {
            cout << "A is destoryed!\n";
        }
};

class B:public A
{
    public:
        virtual void display()
        {
            cout << "I am B\n";
        }
        ~B()
        {
            cout << "B is destoryed!\n";
        }
};

int main(int argc, char *argv[])
{
    A * _ptr = new B();
    _ptr->display();
    delete _ptr;

    return 0;
}
