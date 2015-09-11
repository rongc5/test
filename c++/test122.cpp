#include <iostream>

using namespace std;

class A
{
    protected:
        void display()
        {
            cout << "hello world\n";
        }
};

class B:public A
{
    public:
        void show()
        {
            //A::display();
            display();
        }
};

int main(int argc, char *argv[])
{
    B b;

    b.show();

    return 0;
}


