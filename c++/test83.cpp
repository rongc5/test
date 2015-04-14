#include <iostream>
#include <string>

using namespace std;

class A
{
    public:
        A(){};
        A(int a, string & b):a(a), b(b){}
        void show();

    private:
        int a;
        string b;
};

void A::show()
{
    cout << "a: " << a << "b: " << b.c_str() << '\n';
}


int main(int argc, char *argv[])
{
    string str = "hello world";

    A a(1, str);
    A & b = a;

    b.show();

    A * ptr = &b;

    ptr->show();

    return 0;
}
