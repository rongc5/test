#include <iostream>
#include <string>

class A{
public:
    A(){str = "hello world"; std::cout << str.c_str() << '\n';}
private:
    std::string str;
};

class B{
    public:
        B(int b=9):b(b){std::cout << "b " << b << '\n';}
    private:
        int b;
        A a;
};


int main(int argc, char *argv[])
{
    B b;

    return 0;
}
