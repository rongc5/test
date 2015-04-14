#include <iostream>
#include <string.h>
#include <string>

using namespace std;

class B{
    public:
        void show(){cout << "hello"<<endl;}
};



class A{
    public:
        A(const int & a, const string &b):a(a), b(b){ cout << "a:" << b << endl;this->show();}
        ~A(){}
        void show(){cout << "a: " << a <<" "<< "b: " << b << endl;}
        /*void show(){B::show();}*/ //error
    private:
        int a;
        string b;
};




int main(int argc, char *argv[])
{
    int b= 6;
    cout << "b:" << &b << endl;
    A a(b, "hello");
   
    a.show();


    return 0;
}
