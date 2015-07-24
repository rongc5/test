#include <iostream>
#include "nest.h"
#include <string>

using namespace std;

class A::B
{
    private:
        string str;
    public:
        B(const char * ptr):str(ptr){}
        ~B(){}
        void operator()();
};

void A::B::operator()()
{
    cout << str << '\n';
}


A::A(const char *ptr)
{
    m_b = new B(ptr);
}

A::~A()
{
    delete m_b;
}

void A::show()
{
    if (m_b)
    {
        //m_b->operator()();
        (*m_b)();
    }
}


