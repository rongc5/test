#include <iostream>
#include <stdio.h>

using namespace std;

class B{
public:
    virtual void Print(){cout << "The type is B" << endl;}
};

class D1:public B
{
    public:
        void Print(){cout << "the type is D1" << endl;}
};

class D2:public B
{
public:
    void Print(){cout << "the type is D2" << endl;}
};

int main()
{
    B *pB;
    B b;
    D1 d1;
    D2 d2;

    pB = &b;
    pB->Print();

    pB = &d1;
    pB->Print();

    pB = &d2;
    pB->Print();

    return 0;
}
