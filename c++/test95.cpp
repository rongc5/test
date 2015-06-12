#include <iostream>
#include <string>
#include <vector>

using namespace std;

class A
{
    public:
        static void display();
        static int a;
};

int A::a = 99;


void A::display()
{
    cout << "hello world!!!" << '\n';
}

class B:public A
{
    public:
};


int main(int argc, char *argv[])
{
    
    cout << B::a << '\n';
    B::display();


    return 0;
}
