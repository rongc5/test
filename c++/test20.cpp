#include <iostream>
#include <string.h>
#include <string>

using namespace std;

class B{
    public:
        virtual int show()=0;
};

class A:public B{
    public:
        A(const int & a, const string &b):a(a), b(b){}
        ~A(){}
        virtual int show(){cout << "a: " << a <<" "<< "b: " << b << endl; return 0;}
        /*void show(){B::show();}*/ //error
    private:
        int a;
        string b;
};

int main(int argc, char *argv[])
{
    int b= 6;
   
    B *p = new A(b, "hello");

    p->show();

    return 0;
}
