#include <iostream>

class A{
public:
    void show(int a, int b, const int c = 200);
};

void A::show(int a, int b, const int c)
{
    std::cout << a << " " << b << " " << c << '\n';
}

int main(int argc, char *argv[])
{
    A a;
    a.show(11, 22);

    return 0;
}
