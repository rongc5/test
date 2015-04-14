#include <iostream>
#include <string>

class A{
    public:
        A(int a=200, int b=300):a(a), b(b){}
        void show(){std::cout << a <<' ' << b << ' ' << '\n';}
        void set(int a){this->b = a;}

private:
    const int a;
    int b;
};

int main(int argc, char *argv[])
{
    A a(900);

    a.show();

    a.set(900);
    a.show();
    return 0;
}
