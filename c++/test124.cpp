#include <iostream>

using namespace std;

class base
{
    public:
        virtual void display()=0;
        virtual ~base(){}
};

class A:public base
{
    public:
        virtual void display()
        {
            cout << "hello, I am A!\n";
        }
};

int main(int argc, char *argv[])
{
    A a;
    base *b = &a;
    b->display();

    return 0;
}
