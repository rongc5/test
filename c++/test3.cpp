#include <iostream>
#include <string>


using namespace std;

class A{
    public:
    virtual void f(){cout << "A::f" << endl;}
};

class B:public A{
    public:
        virtual void f(){cout << "B::f" << endl;}
};

class C:public A{
    public:
        virtual void f(){cout<< "C::f" << endl;}
};

class D: public C{
    public:
        virtual void f(){cout << "D::f" << endl;}
};

int main(void)
{
    A a;
    B b;
    C c;
    D d;
    a.f();
    b.f();
    c.f();
    d.f();

    return 0;
}
