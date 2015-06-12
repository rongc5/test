#include <iostream>
#include <string.h>
#include <string>
#include <list>
#include <map>

using namespace std;

int a = 99;

class B{
    public:
        void virtual show()= 0;
};



class A: public virtual B{
    public:
        A(const int & a, const string &b):a(a), b(b){ cout << "a:" << b << endl;this->show();}
        ~A(){}
        void virtual show(){cout << "a: " << a <<" "<< "b: " << b << endl;}
        /*void show(){B::show();}*/ //error
    private:
        int a;
        string b;
};




int main(int argc, char *argv[])
{
    A a(9, "hello");
    a.show();

    return 0;
}
