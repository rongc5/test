#include <iostream>
#include <string.h>

using namespace std;

class A{
    public:
        A(int & a, char *b):a(a){ strcpy(this->b, b); cout << "a:" << &b << endl;}
        ~A(){}
        void show(){cout << "a: " << a <<" "<< "b: " << b << endl;}
    private:
        int a;
        char b[32];
};


int main(int argc, char *argv[])
{
    int b= 6;
    cout << "b:" << &b << endl;
    A a(b, "hello");
   
    a.show();


    return 0;
}
