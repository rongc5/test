#include <iostream>

using namespace std;

class A{
    public:
        virtual void display(){
            cout << "I am is A\n";
        }
        virtual ~A(){}
};

class B : public A{
    public:
        void display() {
            cout << "I am is B\n";
            A::display();
        }
};

int main(int argc, char *argv[])
{
    A * a =  new B();

    a->display();


    return 0;
}
