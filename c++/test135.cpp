#include <iostream>
#include <string>

using namespace std;


class A{
    public:
        A(){}
        virtual ~A(){}
        //virtual void run()=0;
        virtual void run(){};
};

class B: public A
{
    public:
        B(){}
        ~B(){}
        //virtual void run();
        void run();
};

void B::run()
{
    cout << "hello world\n";
}

int main(int argc, char *argv[])
{
    B b;
    b.run();
    return 0;
}
